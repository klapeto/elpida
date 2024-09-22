//
// Created by klapeto on 7/4/2024.
//

#include "SvgCalculateTask.hpp"
#include "Elpida/Core/SimpleTaskData.hpp"

namespace Elpida
{
	void SvgCalculateTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
		auto ptr = dynamic_cast<SimpleTaskData<SvgDocument>*>(_inputData.get());

		_inputDocument = std::move(ptr->GetUnderlyingData());
	}

	SharedPtr<AbstractTaskData> SvgCalculateTask::Finalize()
	{
		return CreateData<SimpleTaskData<SvgCalculatedDocument>>(std::move(_calculatedDocument),
				_inputData->GetAllocator());
	}

	static Size GetElementCount(const SvgElement& element)
	{
		Size elements = element.GetChildren().size() + 1;

		for (auto& child : element.GetChildren())
		{
			elements += GetElementCount(child);
		}
		return elements;
	}

	Size SvgCalculateTask::GetProcessedDataSize() const
	{
		return GetElementCount(_inputDocument.GetElement());
	}

	void SvgCalculateTask::DoRunImpl()
	{
		auto& document = _inputDocument;
		auto scale = _scale;
		Exec([&]()
		{
			_calculatedDocument = SvgCalculatedDocument(document, scale);
		});
	}

	SvgCalculateTask::SvgCalculateTask(double scale)
			:_scale(scale)
	{
		if (_scale <= 0.0)
		{
			throw ElpidaException("Invalid SVG scale: ", _scale);
		}
	}

	TaskInfo SvgCalculateTask::DoGetInfo() const
	{
		return { "SVG Calculation",
				 "Calculates lengths and transformations and scales the values to the desired scale",
				 "Elements",
				 "How many elements are processes in the time",
				 ResultType::Throughput
		};
	}

	Size SvgCalculateTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> SvgCalculateTask::DoDuplicate() const
	{
		return std::make_unique<SvgCalculateTask>(_scale);
	}
} // Elpida