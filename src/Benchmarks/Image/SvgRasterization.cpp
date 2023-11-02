//
// Created by klapeto on 31/10/2023.
//

#include "SvgRasterization.hpp"
#include "AllocatorData.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "ImageTaskData.hpp"

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"

namespace Elpida
{
	TaskInfo SvgRasterization::GetInfo() const
	{
		return TaskInfo("SVG Rasterization",
			"Rasterizes a parsed SVG to an RGBA array",
			"Pixels",
			"How many pixels per second are rasterized.",
			ScoreType::Throughput);
	}

	void SvgRasterization::Prepare(UniquePtr<AbstractTaskData> inputData)
	{
		if (inputData->GetSize() != sizeof(NSVGimage*))
		{
			throw ElpidaException("Svg rasterization must be supplied with data. It got zero size data.");
		}

		_imageData = std::move(inputData);

		_image = *reinterpret_cast<NSVGimage**>(_imageData->GetData());

		_outputData = std::make_unique<ImageTaskData>(_imageData->GetTargetProcessor(),
			_imageData->GetAllocator(),
			_image->width * _scale,
			_image->height * _scale,
			4,
			sizeof(u_int8_t));

		_rasterizer = nsvgCreateRasterizer();

		allocatorData.allocator = &_imageData->GetAllocator();
		allocatorData.targetProcessor = &_imageData->GetTargetProcessor();
	}

	void SvgRasterization::DoRun()
	{
		nsvgRasterize(_rasterizer, _image, 0, 0, _scale, _outputData->GetData(), _image->width * _scale, _image->height * _scale, _image->width * 4 * _scale);
	}

	UniquePtr<AbstractTaskData> SvgRasterization::Finalize()
	{
		return std::move(_outputData);
	}

	bool SvgRasterization::CanBeMultiThreaded() const
	{
		return false;
	}

	UniquePtr<Task> SvgRasterization::DoDuplicate() const
	{
		return std::make_unique<SvgRasterization>(_scale);
	}

	SvgRasterization::~SvgRasterization()
	{
		nsvgDeleteRasterizer(_rasterizer);
	}

	SvgRasterization::SvgRasterization(float scale)
		: _image(nullptr), _rasterizer(nullptr), _scale(scale)
	{

	}

	Size SvgRasterization::GetProcessedDataSize() const
	{
		return _image->width * _scale * _image->height * _scale;
	}
} // Elpida