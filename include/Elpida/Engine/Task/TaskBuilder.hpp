/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 9/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKBUILDER_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKBUILDER_HPP

#include <unordered_map>
#include <vector>
#include <Elpida/Engine/Calculators/TaskResultCalculator.hpp>
#include "Elpida/Config.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"
#include "Elpida/Engine/Configuration/ConfigurationInstance.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{

	class TaskSpecification;
	class Task;
	class TaskConfiguration;
	class TaskAffinity;
	class ConfigurationSpecificationBase;
	class ConfigurationValueBase;

	class TaskBuilder
	{
	public:

		TaskBuilder& canBeDisabled(bool canBeDisabled = true);
		TaskBuilder& canBeMultiThreaded(bool canBeMultiThreaded = true);
		TaskBuilder& shouldBeCountedOnResults(bool shouldBeCountedOnResults = true);

		TaskBuilder& withIterationsToRun(size_t iterationsToRun)
		{
			if (iterationsToRun != 0)
			{
				_iterationsToRun = iterationsToRun;
				return *this;
			}
			else
			{
				throw ElpidaException(FUNCTION_NAME, "iterations must be at least 1");
			}
		}

		TaskBuilder& withTaskResultCalculator(TaskResultCalculator* taskResultCalculator)
		{
			_taskResultCalculator = taskResultCalculator;
			return *this;
		}

		template<typename T>
		TaskBuilder& withFixedConfiguration(const std::string& name, const T& value)
		{
			addPredefinedConfigurationValue(name, value, true);
			return *this;
		}

		template<typename T>
		TaskBuilder& withDefaultConfiguration(const std::string& name, const T& value)
		{
			addPredefinedConfigurationValue(name, value, false);
			return *this;
		}

		TaskConfiguration getDefaultConfiguration() const
		{
			return generateDefaultConfiguration();
		}

		[[nodiscard]] const TaskResultCalculator* getTaskResultCalculator() const
		{
			return _taskResultCalculator;
		}

		[[nodiscard]] const TaskSpecification& getTaskSpecification() const
		{
			return *_taskSpecification;
		}

		[[nodiscard]] Task* build(const TaskConfiguration& configuration, const ProcessorNode& processorToRun) const;

		bool isShouldBeCountedOnResults() const
		{
			return _shouldBeCountedOnResults;
		}

		bool isCanBeMultiThreaded() const
		{
			return _canBeMultiThreaded;
		}

		bool isCanBeDisabled() const
		{
			return _canBeDisabled;
		}

		size_t getIterationsToRun() const
		{
			return _iterationsToRun;
		}

		explicit TaskBuilder(TaskSpecification& specification)
			:
			_taskSpecification(&specification),
			_taskResultCalculator(nullptr),
			_iterationsToRun(1),
			_shouldBeCountedOnResults(false),
			_canBeMultiThreaded(false),
			_canBeDisabled(false)
		{
			fillConfigurationMap();
		}

		virtual ~TaskBuilder();
	private:
		std::unordered_map<std::string, ConfigurationSpecificationBase*> _configurationMap;
		std::unordered_map<std::string, ConfigurationInstance> _predefinedConfigurationsValues;
		TaskSpecification* _taskSpecification;
		TaskResultCalculator* _taskResultCalculator;
		size_t _iterationsToRun;
		bool _shouldBeCountedOnResults;
		bool _canBeMultiThreaded;
		bool _canBeDisabled;

		void fillConfigurationMap();

		TaskConfiguration generateDefaultConfiguration() const;

		template<typename T>
		void addPredefinedConfigurationValue(const std::string& name, const T& value, bool fixed)
		{
			auto itr = _configurationMap.find(name);
			if (itr != _configurationMap.end())
			{
				// TODO: Type checking
				auto existing = _predefinedConfigurationsValues.find(name);
				if (existing != _predefinedConfigurationsValues.end())
				{
					_predefinedConfigurationsValues.erase(existing);
				}
				_predefinedConfigurationsValues.emplace(name,
					ConfigurationInstance(*itr->second, new ConfigurationValue<T>(*itr->second, value, fixed), fixed));
			}
			else
			{
				throw ElpidaException(FUNCTION_NAME,
					Vu::Cs("Setting '",
						name,
						"' is not valid for this Task Specification: '",
						_taskSpecification->getName(),
						"'"));
			}
		}
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKBUILDER_HPP
