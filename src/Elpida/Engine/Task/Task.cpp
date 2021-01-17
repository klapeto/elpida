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

namespace Elpida
{

	Task::Task(const TaskSpecification& specification, const ProcessorNode& processorToRun, size_t iterationsToRun)
		: _processorToRun(processorToRun),
		  _specification(specification),
		  _taskData(nullptr),
		  _iterationsToRun(iterationsToRun)
	{

	}

	void Task::applyAffinity()
	{
		OsUtilities::setCurrentThreadAffinity((int)_processorToRun.getOsIndex());
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

	static size_t getInputDataSize(const TaskDataDto* input)
	{
		return input && input->hasData() ? input->getTaskData()->getSize() : 0;
	}

	TaskResult Task::calculateTaskResult(const Duration& taskElapsedTime) const
	{
		return TaskResult(_specification,
			TaskMetrics(taskElapsedTime,
				calculateTaskResultValue(taskElapsedTime),
				getInputDataSize(_taskData)));
	}
}