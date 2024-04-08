//
// Created by klapeto on 8/4/2024.
//

#include "SvgRasterizationSingleThreadedTask.hpp"
#include "Elpida/Core/SimpleTaskData.hpp"
#include "Elpida/Svg/SvgRasterizer.hpp"
#include "ImageTaskData.hpp"

namespace Elpida
{
	bool SvgRasterizationSingleThreadedTask::CanBeMultiThreaded() const
	{
		return false;
	}

	void SvgRasterizationSingleThreadedTask::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);

		auto ptr = dynamic_cast<SimpleTaskData<SvgCalculatedDocument>*>(_inputData.get());

		_inputDocument = std::move(ptr->GetUnderlyingData());
	}

	UniquePtr<AbstractTaskData> SvgRasterizationSingleThreadedTask::Finalize()
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

	Size SvgRasterizationSingleThreadedTask::GetProcessedDataSize() const
	{
		return _inputDocument.GetViewPort().GetWidth() * _inputDocument.GetViewPort().GetHeight();
	}

	void SvgRasterizationSingleThreadedTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			_rasterizedImage = SvgRasterizer::Rasterize(_inputDocument, _subSamples);
		}
	}

	SvgRasterizationSingleThreadedTask::SvgRasterizationSingleThreadedTask(std::size_t subSamples)
			: _subSamples(subSamples)
	{

	}

	TaskInfo SvgRasterizationSingleThreadedTask::DoGetInfo() const
	{
		return { "Svg Rasterization (Single Thread)",
				 "Rasterizes an calculated Svg document using 1 thread.",
				 "pixels",
				 "How many pixels per second are rasterized on average.",
				 ScoreType::Throughput };
	}

	Size SvgRasterizationSingleThreadedTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration SvgRasterizationSingleThreadedTask::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> SvgRasterizationSingleThreadedTask::DoDuplicate() const
	{
		return std::make_unique<SvgRasterizationSingleThreadedTask>(_subSamples);
	}
} // Elpida