//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Task/Task.hpp"

#include "Elpida/Config.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Engine/Task/TaskThread.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Data/DataAdapter.hpp"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Engine/Data/PassiveTaskData.hpp"

namespace Elpida
{

	Task::Task(const TaskSpecification& specification, const ProcessorNode& processorToRun)
		: _processorToRun(processorToRun), _specification(specification)
	{

	}

	void Task::applyAffinity()
	{
		TaskThread::setCurrentThreadAffinity((int)_processorToRun.getOsIndex());
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
		if (_specification.acceptsInput() && _inputData.getTaskData() == nullptr)
		{
			throw ElpidaException(FUNCTION_NAME,
				Vu::Cs("'", _specification.getName(), "' task needs input and it was not provided!"));
		}

		validateDataProperties(_specification.getInputDataSpecification(), _inputData);

		prepareImpl();
	}

	void Task::finalize()
	{
		_outputData = finalizeAndGetOutputData();
		validateDataProperties(_specification.getOutputDataSpecification(), _outputData);
	}

	static size_t getInputDataSize(RawData* input)
	{
		return input != nullptr ? input->getSize() : 0;
	}

	TaskResult Task::calculateTaskResult(const Duration& taskElapsedTime) const
	{
		return TaskResult(_specification,
			TaskMetrics(taskElapsedTime,
				calculateTaskResultValue(taskElapsedTime),
				getInputDataSize(_inputData.getTaskData())));
	}

}