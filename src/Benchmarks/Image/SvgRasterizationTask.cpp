//
// Created by klapeto on 9/4/2024.
//

#include "SvgRasterizationTask.hpp"

#include "Elpida/Core/SimpleTaskData.hpp"
#include "Elpida/Svg/SvgDirectRasterizer.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "ImageTaskData.hpp"
#include "Utilities.hpp"

namespace Elpida
{

	void SvgRasterizationTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);

		auto ptr = dynamic_cast<SimpleTaskData<SvgCalculatedDocument>*>(_inputData.get());

		_inputDocument = &ptr->GetUnderlyingData();
		_rasterizedImage = SvgBackDrop(std::ceil(_inputDocument->GetViewPort().GetWidth()), std::ceil(_inputDocument->GetViewPort().GetHeight()));
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

	Size SvgRasterizationTask::GetProcessedDataSize() const
	{
		return Utilities::CalculateTotalPixelsRendered(_inputDocument->GetRootShape());
	}

	void SvgRasterizationTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			 SvgDirectRasterizer::Rasterize(*_inputDocument, _rasterizedImage, _superSampler);
		}
	}

	SvgRasterizationTask::SvgRasterizationTask(std::size_t subSamples)
			:  _inputDocument(nullptr)
	{
		_superSampler = SvgSuperSampler(subSamples);
	}

	TaskInfo SvgRasterizationTask::DoGetInfo() const
	{
		return { "Svg Rasterization",
				 "Rasterizes a calculated Svg document",
				 "pixels",
				 "How many pixels per second are calculated on average",
				 ResultType::Throughput
		};
	}

	Size SvgRasterizationTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> SvgRasterizationTask::DoDuplicate() const
	{
		return std::make_unique<SvgRasterizationTask>(_superSampler.GetSampleCount());
	}
} // Elpida