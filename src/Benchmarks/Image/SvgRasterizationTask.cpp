//
// Created by klapeto on 9/4/2024.
//

#include "SvgRasterizationTask.hpp"

#include "Elpida/Core/SimpleTaskData.hpp"
#include "Elpida/Svg/SvgRasterizer.hpp"
#include "ImageTaskData.hpp"

namespace Elpida
{

	void SvgRasterizationTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);

		auto ptr = dynamic_cast<SimpleTaskData<SvgCalculatedDocument>*>(_inputData.get());

		_inputDocument = &ptr->GetUnderlyingData();
	}

	SharedPtr<AbstractTaskData> SvgRasterizationTask::Finalize()
	{
		auto ptr = std::make_unique<ImageTaskData>(_inputData->GetAllocator(), _rasterizedImage.GetWidth(), _rasterizedImage.GetHeight(), 4, 4);

		ptr->Allocate(_rasterizedImage.GetColorData().size() * 4 * 4);
		auto data = reinterpret_cast<float*>(ptr->GetData());
		auto& colorData = _rasterizedImage.GetColorData();
		for (std::size_t i = 0; i < colorData.size(); ++i)
		{
			auto colorIndex = i * 4;
			auto& color = colorData[i];
			data[colorIndex] = color.R();
			data[colorIndex + 1] = color.G();
			data[colorIndex + 2] = color.B();
			data[colorIndex + 3] = color.A();
		}

		return ptr;
	}

	static std::size_t CalculateShapeCount(const SvgCalculatedShape& shape)
	{
		std::size_t size = 0;
		if (!shape.GetPaths().empty() && shape.GetOpacity() > 0.0 && (shape.GetFill().has_value() || shape.GetStroke().has_value()))
		{
			size++;
		}

		for (auto& child : shape.GetChildren())
		{
			size += CalculateShapeCount(child);
		}

		return size;
	}


	Size SvgRasterizationTask::GetProcessedDataSize() const
	{
		return CalculateShapeCount(_inputDocument->GetRootShape());
	}

	void SvgRasterizationTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			_rasterizedImage = SvgRasterizer::Rasterize(*_inputDocument, _subSamples);
		}
	}

	SvgRasterizationTask::SvgRasterizationTask(std::size_t subSamples)
			:  _inputDocument(nullptr), _subSamples(subSamples)
	{

	}

	TaskInfo SvgRasterizationTask::DoGetInfo() const
	{
		return { "Svg Rasterization",
				 "Rasterizes a calculated Svg document",
				 "shapes",
				 "How many shapes per second are calculated on average",
				 ScoreType::Throughput
		};
	}

	Size SvgRasterizationTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration SvgRasterizationTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> SvgRasterizationTask::DoDuplicate() const
	{
		return std::make_unique<SvgRasterizationTask>(_subSamples);
	}
} // Elpida