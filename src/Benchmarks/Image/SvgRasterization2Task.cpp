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
// Created by klapeto on 7/9/2024.
//

#include "SvgRasterization2Task.hpp"

#include "Elpida/Core/SimpleTaskData.hpp"
#include "Elpida/Svg/SvgRasterizer.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "ImageTaskData.hpp"
#include "Utilities.hpp"

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
				_rasterizedImage.GetHeight(), 4, sizeof(Float));

		ptr->Allocate(_rasterizedImage.GetColorData().size() * 4 * sizeof(Float));
		auto data = reinterpret_cast<Float*>(ptr->GetData());
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


	Size SvgRasterization2Task::GetProcessedDataSize() const
	{
		return Utilities::CalculateTotalPixelsRendered(_inputDocument->GetRootShape());
	}

	void SvgRasterization2Task::DoRunImpl()
	{
		Exec([&]()
		{
			_rasterizedImage = SvgRasterizer::Rasterize(*_inputDocument, _superSampler, _multiThread);
		});
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
				 "pixels",
				 "How many pixels per second are calculated on average",
				 ResultType::Throughput
		};
	}

	Size SvgRasterization2Task::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> SvgRasterization2Task::DoDuplicate() const
	{
		return std::make_unique<SvgRasterization2Task>(_superSampler.GetSampleCount(), _multiThread);
	}
} // Elpida