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

	SvgCalculateTask::SvgCalculateTask(SvgFloat scale)
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