//
// Created by klapeto on 7/9/2024.
//

#include "SvgRasterization2Task.hpp"

#include "Elpida/Core/SimpleTaskData.hpp"
#include "Elpida/Svg/SvgRasterizer.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "ImageTaskData.hpp"

namespace Elpida
{

	void SvgRasterization2Task::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);

		auto ptr = dynamic_cast<SimpleTaskData<SvgCalculatedDocument>*>(_inputData.get());

		_inputDocument = &ptr->GetUnderlyingData();
	}

	SharedPtr<AbstractTaskData> SvgRasterization2Task::Finalize()
	{
		auto ptr = std::make_unique<ImageTaskData>(_inputData->GetAllocator(), _rasterizedImage.GetWidth(),
				_rasterizedImage.GetHeight(), 4, 4);

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
		if (!shape.GetPaths().empty() && shape.GetOpacity() > 0.0 &&
			(shape.GetFill().has_value() || shape.GetStroke().has_value()))
		{
			size++;
		}

		for (auto& child : shape.GetChildren())
		{
			size += CalculateShapeCount(child);
		}

		return size;
	}

	Size SvgRasterization2Task::GetProcessedDataSize() const
	{
		return CalculateShapeCount(_inputDocument->GetRootShape());
	}

	void SvgRasterization2Task::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			_rasterizedImage = SvgRasterizer::Rasterize(*_inputDocument, _superSampler, _multiThread);
		}
	}

	SvgRasterization2Task::SvgRasterization2Task(std::size_t subSamples, bool multiThread)
			:_inputDocument(nullptr), _multiThread(multiThread)
	{
		_superSampler = SvgSuperSampler(subSamples);
	}

	TaskInfo SvgRasterization2Task::DoGetInfo() const
	{
		return { "Svg Rasterization 2",
				 "Rasterizes a calculated Svg document. Uses multiple buffers instead of a single.",
				 "shapes",
				 "How many shapes per second are calculated on average",
				 ResultType::Throughput
		};
	}

	Size SvgRasterization2Task::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration SvgRasterization2Task::GetExecutionMinimumDuration()
	{
		return Elpida::Seconds(5);
	}

	UniquePtr<Task> SvgRasterization2Task::DoDuplicate() const
	{
		return std::make_unique<SvgRasterization2Task>(_superSampler.GetSampleCount(), _multiThread);
	}
} // Elpida