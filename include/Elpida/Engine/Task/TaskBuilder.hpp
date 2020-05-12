//
// Created by klapeto on 9/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKBUILDER_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKBUILDER_HPP

#include <utility>
#include <unordered_map>
#include <vector>
#include "Elpida/Config.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"
#include "Elpida/Engine/Configuration/Specification/TaskConfigurationSpecifications.hpp"
#include "Elpida/Engine/Configuration/ConfigurationInstance.hpp"
#include "Elpida/Engine/Configuration/Concrete/ConfigurationValue.hpp"
#include "Elpida/Engine/Data/Adapters/ForwardingAdapter.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{

	class TaskSpecification;
	class Task;
	class TaskConfiguration;
	class TaskAffinity;
	class ConfigurationSpecificationBase;
	class ConfigurationValueBase;
	class DataAdapter;

	class TaskBuilder
	{
	public:

		TaskBuilder& canBeDisabled(bool canBeDisabled = true);
		TaskBuilder& canBeMultiThreaded(bool canBeMultiThreaded = true);
		TaskBuilder& shouldBeCountedOnResults(bool shouldBeCountedOnResults = true);

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

		[[nodiscard]] const TaskSpecification& getTaskSpecification() const
		{
			return *_taskSpecification;
		}

		[[nodiscard]] Task* build(const TaskConfiguration& configuration, const TaskAffinity& affinity) const;

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

		const DataAdapter& getDataAdapter() const
		{
			return *_dataAdapter;
		}

		explicit TaskBuilder(TaskSpecification& specification)
			:
			_taskSpecification(&specification),
			_dataAdapter(nullptr),
			_shouldBeCountedOnResults(false),
			_canBeMultiThreaded(false),
			_canBeDisabled(false)
		{
			fillConfigurationMap();
			setNewDataAdapter(*new ForwardingAdapter());
		}
		virtual ~TaskBuilder();
	private:
		std::unordered_map<std::string, ConfigurationSpecificationBase*> _configurationMap;
		std::unordered_map<std::string, ConfigurationInstance> _predefinedConfigurationsValues;
		TaskSpecification* _taskSpecification;
		DataAdapter* _dataAdapter;
		bool _shouldBeCountedOnResults;
		bool _canBeMultiThreaded;
		bool _canBeDisabled;

		void fillConfigurationMap();

		void setNewDataAdapter(DataAdapter& newDataAdapter);

		TaskConfiguration generateDefaultConfiguration() const;

		template<typename T>
		void addPredefinedConfigurationValue(const std::string& name, const T& value, bool fixed)
		{
			auto itr = _configurationMap.find(name);
			if (itr != _configurationMap.end())
			{
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
