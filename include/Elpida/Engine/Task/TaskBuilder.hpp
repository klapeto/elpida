//
// Created by klapeto on 9/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKBUILDER_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKBUILDER_HPP

#include <utility>
#include <unordered_map>
#include <vector>
#include "Elpida/Engine/Configuration/Specification/TaskConfigurationSpecifications.hpp"
#include "Elpida/Engine/Configuration/ConfigurationInstance.hpp"
#include "Elpida/Engine/Data/Adapters/ForwardingAdapter.hpp"

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

		/**
		 * Applies a fixed configuration for this builder. Fixed configuration becomes not configurable
		 * and overrides the global.
		 *
		 * @note The configuration lifetime is passed to the builder.
		 * @param name The name of the configuration
		 * @param configuration The configuration value (Lifetime is passed to the builder)
		 * @return The same TaskBuilder
		 */
		TaskBuilder& withFixedConfiguration(const std::string& name, ConfigurationValueBase& configuration);

		TaskBuilder& withDefaultConfiguration(const std::string& name, ConfigurationValueBase& configuration);

		const TaskConfiguration& getDefaultConfiguration() const
		{
			return _defaultConfiguration;
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
			_defaultConfiguration(specification, *this),
			_taskSpecification(&specification),
			_dataAdapter(nullptr),
			_shouldBeCountedOnResults(false),
			_canBeMultiThreaded(false),
			_canBeDisabled(false)
		{
			fillConfigurationMap();
			setNewDataAdapter(*new ForwardingAdapter());
			generateDefaultConfiguration();
		}
		virtual ~TaskBuilder();
	private:
		std::unordered_map<std::string, ConfigurationSpecificationBase*> _configurationMap;
		std::unordered_map<std::string, ConfigurationInstance> _predefinedConfigurationsValues;
		TaskConfiguration _defaultConfiguration;
		TaskSpecification* _taskSpecification;
		DataAdapter* _dataAdapter;
		bool _shouldBeCountedOnResults;
		bool _canBeMultiThreaded;
		bool _canBeDisabled;

		void fillConfigurationMap();

		void setNewDataAdapter(DataAdapter& newDataAdapter);
		void addPredefinedConfigurationValue(const std::string& name, ConfigurationValueBase& value, bool fixed);
		void generateDefaultConfiguration();
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKBUILDER_HPP
