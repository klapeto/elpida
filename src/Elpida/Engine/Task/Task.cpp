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

#include "Elpida/Engine/Task/Task.hpp"

#include "Elpida/Config.hpp"
#include "Elpida/SystemInfo/ProcessorNode.hpp"
#include "Elpida/Utilities/OsUtilities.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Engine/Runner/DefaultTaskRunner.hpp"
#include "Elpida/ServiceProvider.hpp"
#include "Elpida/SystemInfo/TimingInfo.hpp"

namespace Elpida
{

	Task::Task(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun)
		: _processorToRun(processorToRun),
		  _specification(specification),
		  _serviceProvider(serviceProvider),
		  _taskData(nullptr),
		  _iterationsToRun(iterationsToRun)
	{

	}

	static void validateDataProperties(const DataSpecification& specification, const TaskDataDto& taskDataDto)
	{
		auto& propertiesNames = specification.getRequiredPropertiesNames();
		auto& definedProperties = taskDataDto.getDefinedProperties();
		for (auto& propertyName: propertiesNames)
		{
			if (definedProperties.find(propertyName) == definedProperties.end())
			{
				throw ElpidaException(FUNCTION_NAME,
					Vu::Cs("'", propertyName, "' property was not defined!"));
			}
		}
	}

	void Task::prepare()
	{
		OsUtilities::setCurrentThreadAffinity((int)_processorToRun.getOsIndex());
		if (_specification.acceptsInput())
		{
			if (!_taskData || (_taskData && !_taskData->hasData()))
			{
				throw ElpidaException(FUNCTION_NAME,
					Vu::Cs("'", _specification.getName(), "' task needs input and it was not provided!"));
			}

			validateDataProperties(_specification.getInputDataSpecification(), *_taskData);
		}

		prepareImpl();
	}

	void Task::finalize()
	{
		if (_taskData)
		{
			auto output = finalizeAndGetOutputData();
			if (output.has_value())
			{
				*_taskData = std::move(output.value());
				validateDataProperties(_specification.getOutputDataSpecification(), *_taskData);
			}
		}
	}

	double Task::getInputDataSize() const
	{
		return _taskData && _taskData->hasData() ? _taskData->getTaskData()->getSize() : 0.0;
	}

	void Task::prepareImpl()
	{
	}

	std::optional<TaskDataDto> Task::finalizeAndGetOutputData()
	{
		return std::nullopt;
	}

	TaskDataDto Task::propagateInput()
	{
		return TaskDataDto(std::move(*_taskData));
	}

	TaskDataDto& Task::getInput() const
	{
		return *_taskData;
	}

	void Task::preProcessExecutionParameters(ExecutionParameters& parameters)
	{

	}
}