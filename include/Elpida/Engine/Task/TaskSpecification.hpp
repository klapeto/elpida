//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKSPECIFICATION_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKSPECIFICATION_HPP

#include <string>
#include <Elpida/Engine/Result/ResultSpecification.hpp>
#include "Elpida/Engine/Task/Data/DataSpecification.hpp"
#include "Elpida/Engine/Configuration/ConfigurationSpecification.hpp"
#include "Elpida/Engine/Task/TaskAffinity.hpp"
#include "Elpida/Engine/Configuration/ConfigurationValue.hpp"
#include "Elpida/Engine/Configuration/TaskConfiguration.hpp"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"

namespace Elpida
{

	class Task;
	class TaskConfiguration;

	/**
	 * Standard information regarding a Task
	 */
	class TaskSpecification
	{
	public:
		[[nodiscard]] const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]] const std::string& getDescription() const
		{
			return _description;
		}

		[[nodiscard]] const std::vector<ConfigurationSpecificationBase*>& getConfigurationSpecifications() const
		{
			return _configurationSpecifications;
		}

		[[nodiscard]] bool shouldBeCountedOnResults() const
		{
			return _shouldBeCountedOnResults;
		}

		[[nodiscard]] bool canBeDisabled() const
		{
			return _canBeDisabled;
		}

		[[nodiscard]] const std::string& getId() const
		{
			return _id;
		}

		[[nodiscard]] bool acceptsInput() const
		{
			return _acceptsInput;
		}

		[[nodiscard]] bool exportsOutput() const
		{
			return _exportsOutput;
		}

		[[nodiscard]] bool isMultiThreadingEnabled() const
		{
			return _multiThreadingEnabled;
		}

		const DataSpecification& getInputDataSpecification() const
		{
			return _inputDataSpecification;
		}

		const DataSpecification& getOutputDataSpecification() const
		{
			return _outputDataSpecification;
		}

		const ResultSpecification& getResultSpecification() const
		{
			return _resultSpecification;
		}

		[[nodiscard]] Task* createNewTask(const TaskConfiguration& configuration, const TaskAffinity& affinity) const;

		virtual ~TaskSpecification();
	private:
		std::string _id;
		std::string _name;
		std::string _description;
		DataSpecification _inputDataSpecification;
		DataSpecification _outputDataSpecification;
		ResultSpecification _resultSpecification;
		std::vector<ConfigurationSpecificationBase*> _configurationSpecifications;
		std::unordered_map<std::string, ConfigurationValueBase*> _fixedConfiguration;
		bool _acceptsInput;
		bool _exportsOutput;
		bool _shouldBeCountedOnResults;
		bool _multiThreadingEnabled;
		bool _canBeDisabled;
	protected:
		TaskSpecification(std::string name, ResultSpecification resultSpecification);

		virtual Task* createNewTaskImpl(const TaskConfiguration& configuration, const TaskAffinity& affinity) const = 0;

		template<typename T>
		ConfigurationValue<T>& getSettingAndValidate(const TaskConfiguration& configuration,
			const std::string& name,
			ConfigurationType::Type type) const
		{
			auto config = configuration.getConfiguration(name);
			if (config != nullptr)
			{
				if (config->getConfigurationSpecification().getType() == type)
				{
					return config->as<ConfigurationValue<T>>();
				}
				else
				{
					throw ElpidaException(_name,
						Vu::concatenateToString("Bad configuration: the setting with name '",
							name, "' must had wrong type'"));
				}
			}
			else
			{
				throw ElpidaException(_name,
					Vu::concatenateToString("Bad configuration: a setting with name '",
						name, "' must be defined under benchmark settings"));
			}
		}

		void setCanBeDisabled(bool value = true)
		{
			_canBeDisabled = value;
		}

		void setEnabledMultiThreading(bool value = true)
		{
			_multiThreadingEnabled = value;
		}

		void setToBeCountedOnResults(bool value = true)
		{
			_shouldBeCountedOnResults = value;
		}

		template<typename T>
		void withFixedConfiguration(ConfigurationSpecificationBase* specification, T&& fixedValue)
		{
			_configurationSpecifications.push_back(specification);
			_fixedConfiguration.emplace(specification->getName(), new ConfigurationValue<T>(*specification, fixedValue));
		}

		void withConfiguration(ConfigurationSpecificationBase* specification)
		{
			_configurationSpecifications.push_back(specification);
		}

		void withDescription(std::string&& description)
		{
			_description = std::move(description);
		}

		void withInputData(DataSpecification&& inputDataSpecification)
		{
			_acceptsInput = true;
			_inputDataSpecification = std::move(inputDataSpecification);
		}

		void withOutputData(DataSpecification&& inputDataSpecification)
		{
			_exportsOutput = true;
			_inputDataSpecification = std::move(inputDataSpecification);
		}
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKSPECIFICATION_HPP
