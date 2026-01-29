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

#include "SvgParseTask.hpp"
#include "Elpida/Core/SimpleTaskData.hpp"

namespace Elpida
{
	void SvgParseTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
		auto ptr = dynamic_cast<SimpleTaskData<XmlElement>*>(_inputData.get());

		_inputXmlElement = std::move(ptr->GetUnderlyingData());
	}

	SharedPtr<AbstractTaskData> SvgParseTask::Finalize()
	{
		return std::make_shared<SimpleTaskData<SvgDocument>>(std::move(_parsedElement), _inputData->GetAllocator());
	}

	TaskInfo SvgParseTask::DoGetInfo() const
	{
		return { "SVG Parsing",
				 "Parses a SVG document and measures the parsing throughput",
				 "Elements",
				 "How many elements are processed in the time",
				 ResultType::Throughput
		};
	}

	static Size GetElementCount(const XmlElement& element)
	{
		Size elements = element.GetChildren().size() + 1;

		for (auto& child: element.GetChildren())
		{
			elements += GetElementCount(child);
		}
		return elements;
	}

	Size SvgParseTask::GetProcessedDataSize() const
	{
		return GetElementCount(_inputXmlElement);
	}

	void SvgParseTask::DoRunImpl()
	{
		auto& element = _inputXmlElement;
		Exec([&]()
		{
			_parsedElement = SvgDocument(element);
		});
	}

	Size SvgParseTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> SvgParseTask::DoDuplicate() const
	{
		return std::make_unique<SvgParseTask>();
	}
} // Elpida