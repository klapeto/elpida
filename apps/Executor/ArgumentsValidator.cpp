//
// Created by klapeto on 12/3/2023.
//

#include "ArgumentsValidator.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "DefaultFormatter.hpp"

#include <filesystem>

namespace Elpida
{
	void ArgumentsValidator::ValidateAffinity(const Vector<unsigned int>& affinity)
	{
		for (Size i = 0; i < affinity.size(); ++i)
		{
			auto value = affinity[i];
			for (Size j = i + 1; j < affinity.size(); ++j)
			{
				if (affinity[j] == value)
				{
					throw ElpidaException("Duplicate affinity '", value, "' detected. Only unique processors allowed");
				}
			}
		}
	}

	Size ArgumentsValidator::GetBenchmarkIndex() const
	{
		return _benchmarkIndex;
	}

	ArgumentsValidator::ArgumentsValidator(const String& modulePath, const String& benchmarkIndex, const String& affinity, const String& format)
	{
		if (modulePath.empty())
		{
			throw ElpidaException("module path cannot be empty");
		}

		if (!std::filesystem::exists(modulePath))
		{
			throw ElpidaException("module: '", modulePath, "' does not exist");
		}

		if (benchmarkIndex.empty())
		{
			throw ElpidaException("index cannot be empty");
		}
		_benchmarkIndex = std::stoul(benchmarkIndex);

		if (affinity.empty())
		{
			throw ElpidaException("affinity cannot be empty");
		}

		_parsedAffinity = Parse(affinity);

		ValidateAffinity(_parsedAffinity);

		//if (format == "json"){
			_resultFormatter = std::make_unique<DefaultFormatter>();
		//}
	}
	void
	ArgumentsValidator::ValidateAndAssignConfiguration(const Vector<String>& configurationValues, Vector<TaskConfiguration>& taskConfigurations) const
	{
		if (configurationValues.size() != taskConfigurations.size())
		{
			throw ElpidaException("benchmark required ", taskConfigurations.size(), " configurations but were provided ", configurationValues.size());
		}

		for (Size i = 0; i < taskConfigurations.size(); ++i)
		{
			taskConfigurations[i].Parse(configurationValues[i]);
		}
	}
	Vector<Ref<const ProcessingUnitNode>>
	ArgumentsValidator::ValidateAndGetProcessingUnits(const TopologyInfo& topologyInfo) const
	{
		Vector<Ref<const ProcessingUnitNode>> returnVector;
		returnVector.reserve(_parsedAffinity.size());

		auto& processors = topologyInfo.GetAllProcessingUnits();
		for (auto index: _parsedAffinity)
		{
			bool found = false;
			for (auto& processor: processors)
			{
				if (processor.get().GetOsIndex().value() == index)
				{
					returnVector.emplace_back(processor.get());
					found = true;
					break;
				}
			}

			if (!found)
			{
				throw ElpidaException("No processor with id ", index, " was found");
			}
		}

		return returnVector;
	}

	const ResultFormatter& ArgumentsValidator::GetResultFormatter() const
	{
		return *_resultFormatter;
	}
	Vector<unsigned int> ArgumentsValidator::Parse(const String& value)
	{
		Vector<unsigned int> returnVector;
		std::ostringstream accumulator;
		for (Size i = 0; i < value.size(); ++i)
		{
			auto c = value[i];

			if (c == ',')
			{
				auto str = accumulator.str();
				returnVector.push_back(std::stoul(str));
				accumulator.clear();
			}
			else if (std::isdigit(c))
			{
				accumulator << c;
			}
			else
			{
				throw ElpidaException("unexpected character '", (char)c, "' while parsing affinity at pos: ", i);
			}
		}

		auto str = accumulator.str();
		returnVector.push_back(std::stoul(str));

		return returnVector;
	}
} // Elpida