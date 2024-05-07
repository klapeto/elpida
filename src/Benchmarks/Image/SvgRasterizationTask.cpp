//
// Created by klapeto on 9/4/2024.
//

#include "SvgRasterizationTask.hpp"

#include "Elpida/Core/SimpleTaskData.hpp"
#include "Elpida/Svg/SvgRasterizer.hpp"
#include "ImageTaskData.hpp"

namespace Elpida
{
	bool SvgRasterizationTask::CanBeMultiThreaded() const
	{
		return false;
	}

	void SvgRasterizationTask::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);

		auto ptr = dynamic_cast<SimpleTaskData<SvgCalculatedDocument>*>(_inputData.get());

		_inputDocument = std::move(ptr->GetUnderlyingData());
	}

	UniquePtr<AbstractTaskData> SvgRasterizationTask::Finalize()
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

	static Size CalculateTotalPixelsRendered(const SvgCalculatedShape& shape)
	{
		Size size = 0;
		if (!shape.GetPaths().empty() && shape.GetOpacity() > 0.0 && (shape.GetFill().has_value() || shape.GetStroke().has_value()))
		{
			size = shape.GetBounds().GetHeight() * shape.GetBounds().GetWidth();
		}

		for (auto& child : shape.GetChildren())
		{
			size += CalculateTotalPixelsRendered(child);
		}

		return size;
	}

	Size SvgRasterizationTask::GetProcessedDataSize() const
	{
		return CalculateTotalPixelsRendered(_inputDocument.GetRootShape()) * _subSamples;
	}

	void SvgRasterizationTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			_rasterizedImage = SvgRasterizer::RasterizeCustom(_inputDocument, _subSamples, _multiThreadedShapes, _multiThreadedSuperSampling, _multiThreadedBitBlt, *_threadPool);
		}
	}

	SvgRasterizationTask::SvgRasterizationTask(std::size_t subSamples,bool multiThreadedShapes, bool multiThreadedSuperSampling, bool multiThreadedBitBlt, ThreadPool* threadPool)
			: _threadPool(threadPool), _subSamples(subSamples), _multiThreadedShapes(multiThreadedShapes), _multiThreadedSuperSampling(multiThreadedSuperSampling), _multiThreadedBitBlt(multiThreadedBitBlt)
	{

	}

	TaskInfo SvgRasterizationTask::DoGetInfo() const
	{
		return { "Svg Rasterization",
				 "Rasterizes a calculated Svg document",
				 "samples",
				 "How many samples per second are calculated on average",
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
		return std::make_unique<SvgRasterizationTask>(_subSamples, _multiThreadedShapes, _multiThreadedSuperSampling, _multiThreadedBitBlt, _threadPool);
	}
} // Elpida