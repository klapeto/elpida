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
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKSPECIFICATION_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKSPECIFICATION_HPP

#include <string>
#include <memory>
#include "Elpida/Engine/Data/DataTransformer.hpp"
#include "Elpida/Engine/Result/ResultSpecification.hpp"
#include "Elpida/Engine/Data/DataSpecification.hpp"
#include "Elpida/Engine/Configuration/Specification/ConfigurationSpecificationBase.hpp"
#include "Elpida/Engine/Configuration/Specification/ConfigurationSpecification.hpp"
#include "Elpida/Engine/Configuration/Concrete/TaskConfiguration.hpp"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"

namespace Elpida
{

	class Task;
	class ProcessorNode;
	class ServiceProvider;

	/**
	 * Standard information regarding a Task
	 */
	class TaskSpecification
	{
	public:
		[[nodiscard]]
		const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]]
		const std::string& getDescription() const
		{
			return _description;
		}

		[[nodiscard]]
		const std::vector<std::shared_ptr<ConfigurationSpecificationBase>>& getConfigurationSpecifications() const
		{
			return _configurationSpecifications;
		}

		[[nodiscard]]
		bool acceptsInput() const
		{
			return _acceptsInput;
		}

		[[nodiscard]]
		bool producesOutput() const
		{
			return _producesOutput;
		}

		[[nodiscard]]
		const DataSpecification& getInputDataSpecification() const
		{
			return _inputDataSpecification;
		}

		[[nodiscard]]
		const DataSpecification& getOutputDataSpecification() const
		{
			return _outputDataSpecification;
		}

		[[nodiscard]]
		const ResultSpecification& getResultSpecification() const
		{
			return _resultSpecification;
		}

		[[nodiscard]]
		std::shared_ptr<DataTransformer> getDataTransformer() const
		{
			return _dataTransformer;
		}

		[[nodiscard]]
		virtual std::unique_ptr<Task> createNewTask(const TaskConfiguration& configuration,
			const ProcessorNode& processorToRun,
			const ServiceProvider& serviceProvider,
			size_t iterationsToRun) const = 0;

		virtual ~TaskSpecification() = default;
	private:
		std::string _name;
		std::string _description;
		DataSpecification _inputDataSpecification;
		DataSpecification _outputDataSpecification;
		ResultSpecification _resultSpecification;
		std::vector<std::shared_ptr<ConfigurationSpecificationBase>> _configurationSpecifications;
		std::shared_ptr<DataTransformer> _dataTransformer;
		bool _acceptsInput;
		bool _producesOutput;
	protected:
		TaskSpecification(std::string name, ResultSpecification resultSpecification)
			: _name(std::move(name)),
			  _resultSpecification(std::move(resultSpecification)),
			  _dataTransformer(nullptr),
			  _acceptsInput(false),
			  _producesOutput(false)
		{
		}

		template<typename T>
		ConfigurationValue<T>& getSettingAndValidate(const TaskConfiguration& configuration,
			const std::string& name,
			ConfigurationType::Type type) const
		{
			auto config = configuration.getConfiguration(name);
			if (config)
			{
				if (config.value()->getConfigurationSpecification().getType() == type)
				{
					return config.value()->as<ConfigurationValue<T>>();
				}
				else
				{
					throw ElpidaException(_name,
						Vu::Cs("Bad configuration: the setting with name '",
							name, "' must had wrong type'"));
				}
			}
			else
			{
				throw ElpidaException(_name,
					Vu::Cs("Bad configuration: a setting with name '",
						name, "' must be defined under benchmark settings"));
			}
		}

		void withConfiguration(std::shared_ptr<ConfigurationSpecificationBase> specification)
		{
			_configurationSpecifications.push_back(std::move(specification));
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

		void withOutputData(DataSpecification&& outputDataSpecification)
		{
			_producesOutput = true;
			_outputDataSpecification = std::move(outputDataSpecification);
		}

		void withDataPropertiesTransformer(std::shared_ptr<DataTransformer> dataPropertiesTransformer)
		{
			_dataTransformer = std::move(dataPropertiesTransformer);
		}

		friend class TaskBuilder;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKSPECIFICATION_HPP
