//
// Created by klapeto on 31/10/2023.
//

#include "SvgParseTask.hpp"

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/RawTaskData.hpp"

#include "SvgImageData.hpp"

#include <cstring>
#include <iostream>

namespace Elpida
{

	void SvgParseTask::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		if (inputData->GetSize() == 0)
		{
			throw ElpidaException("Parse svg must be supplied with data. It got zero size data.");
		}

		_inputData = std::make_unique<RawTaskData>(
			inputData->GetSize() + 1,
			inputData->GetAllocator());
		std::memcpy(_inputData->GetData(), inputData->GetData(), inputData->GetSize());

		_outputData = std::make_unique<SvgImageData>(nullptr,_inputData->GetAllocator());
	}

	void SvgParseTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			_element = _parser.Parse(reinterpret_cast<const char*>(_inputData->GetData()), _inputData->GetSize());
		}
	}

	UniquePtr<AbstractTaskData> SvgParseTask::Finalize()
	{
		return std::move(_outputData);
	}

	UniquePtr<Task> SvgParseTask::DoDuplicate() const
	{
		return std::make_unique<SvgParseTask>();
	}

	bool SvgParseTask::CanBeMultiThreaded() const
	{
		return false;
	}

	TaskInfo SvgParseTask::GetInfo() const
	{
		return TaskInfo("SVG Parsing",
			"Parses SVG data to node tree.",
			"B",
			"How many bytes are parsed per second",
			ScoreType::Throughput);
	}

	Size SvgParseTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	Size SvgParseTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration SvgParseTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

} // Elpida