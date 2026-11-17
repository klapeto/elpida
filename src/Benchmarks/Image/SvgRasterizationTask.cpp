/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 9/4/2024.
//

#include "SvgRasterizationTask.hpp"

#include "Elpida/Core/SimpleTaskData.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "ImageTaskData.hpp"
#include "Utilities.hpp"

namespace Elpida::Benchmarks::Image
{

	void SvgRasterizationTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);

		auto ptr = dynamic_cast<SimpleTaskData<SvgCalculatedDocument>*>(_inputData.get());

		_inputDocument = &ptr->GetUnderlyingData();
		_rasterizedImage.clear();
	}

	SharedPtr<AbstractTaskData> SvgRasterizationTask::Finalize()
	{
		const auto pixelCount = _rasterizer.GetWidth() * _rasterizer.GetHeight();
		auto ptr = std::make_unique<ImageTaskData>(_inputData->GetAllocator(), _rasterizer.GetWidth(),
				_rasterizer.GetHeight(), 4, sizeof(Float));

		// Straight (non premultiplied) float RGBA, as the other image tasks expect
		ptr->Allocate(pixelCount * 4 * sizeof(Float));
		auto data = reinterpret_cast<Float*>(ptr->GetData());
		for (std::size_t i = 0; i < pixelCount * 4; i += 4)
		{
			const auto* pixel = _rasterizedImage.data() + i;
			const Float alpha = pixel[3] / Float(255.0);
			const Float scale = pixel[3] > 0 ? Float(1.0) / pixel[3] : Float(0.0);
			data[i] = pixel[0] * scale;
			data[i + 1] = pixel[1] * scale;
			data[i + 2] = pixel[2] * scale;
			data[i + 3] = alpha;
		}

		return ptr;
	}

	Size SvgRasterizationTask::GetProcessedDataSize() const
	{
		return Utilities::CalculateTotalPixelsRendered(_inputDocument->GetRootShape());
	}

	void SvgRasterizationTask::DoRunImpl()
	{
		Exec([&]()
		{
			_rasterizer.Rasterize(*_inputDocument, _rasterizedImage);
		});
	}

	SvgRasterizationTask::SvgRasterizationTask()
			:_inputDocument(nullptr)
	{
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
		return std::make_unique<SvgRasterizationTask>();
	}
} // Elpida