//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKSPECIFICATION_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKSPECIFICATION_HPP

#include <string>
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
		[[nodiscard]] const std::string& getInputValueDescription() const
		{
			return _inputValueDescription;
		}
		[[nodiscard]] const std::string& getInputValueUnit() const
		{
			return _inputValueUnit;
		}
		[[nodiscard]] const std::string& getOutputValueDescription() const
		{
			return _outputValueDescription;
		}
		[[nodiscard]] const std::string& getOutputValueUnit() const
		{
			return _outputValueUnit;
		}
		[[nodiscard]] const std::string& getThroughputUnit() const
		{
			return _throughputUnit;
		}
		[[nodiscard]] const std::vector<ConfigurationSpecification>& getConfigurationSpecifications() const
		{
			return _configurationSpecifications;
		}
		[[nodiscard]] bool shouldBeCountedOnResults() const
		{
			return _shouldBeCountedOnResults;
		}

		bool canBeDisabled() const
		{
			return _canBeDisabled;
		}

		const std::string& getId() const
		{
			return _id;
		}

		bool acceptsInput() const
		{
			return _acceptsInput;
		}

		bool exportsOutput() const
		{
			return _exportsOutput;
		}

		bool isMultiThreadingEnabled() const
		{
			return _multiThreadingEnabled;
		}

		[[nodiscard]] virtual Task* createNewTask(const TaskConfiguration& configuration,
			const TaskAffinity& affinity) const = 0;

		virtual ~TaskSpecification() = default;
	private:
		std::string _id;
		std::string _name;
		std::string _description;
		std::string _inputValueDescription;
		std::string _inputValueUnit;
		std::string _outputValueDescription;
		std::string _outputValueUnit;
		std::string _throughputUnit;
		std::vector<ConfigurationSpecification> _configurationSpecifications;
		bool _acceptsInput;
		bool _exportsOutput;
		bool _shouldBeCountedOnResults;
		bool _multiThreadingEnabled;
		bool _canBeDisabled;
	protected:
		TaskSpecification(std::string name,
			std::string description,
			std::string inputValueDescription,
			std::string inputValueUnit,
			std::string outputValueDescription,
			std::string outputValueUnit,
			std::string throughputUnit,
			std::vector<ConfigurationSpecification>&& configurationSpecifications,
			bool acceptsInput,
			bool exportsOutput,
			bool shouldBeCountedOnResults,
			bool enableMultiThreading,
			bool canBeDisabled);

		static constexpr std::string_view _noInputString = "[Task receives no input]";
		static constexpr std::string_view _noOutputString = "[Task produces no output]";

		template<typename T>
		ConfigurationValue<T>& getSettingAndValidate(const TaskConfiguration& configuration,
			const std::string& name,
			ConfigurationValueBase::Type type) const
		{
			auto config = configuration.getConfiguration(name);
			if (config != nullptr)
			{
				if (config->getType() == type)
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
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKSPECIFICATION_HPP
