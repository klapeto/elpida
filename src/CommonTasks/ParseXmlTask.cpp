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

#include "ParseXmlTask.hpp"
#include "Elpida/Core/SimpleTaskData.hpp"
#include "Elpida/Xml/XmlParser.hpp"

namespace Elpida
{

	TaskInfo ParseXmlTask::DoGetInfo() const
	{
		return { "XML Parsing",
				 "Parses an XML document and measures the parsing throughput",
				 "chars",
				 "How many characters are processes in the time",
				 ResultType::Throughput};
	}

	UniquePtr<Task> ParseXmlTask::DoDuplicate() const
	{
		return std::make_unique<ParseXmlTask>();
	}

	void ParseXmlTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> ParseXmlTask::Finalize()
	{
		return std::make_shared<SimpleTaskData<XmlElement>>(std::move(_parsedElement), _inputData->GetAllocator());
	}

	Size ParseXmlTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	void ParseXmlTask::DoRunImpl()
	{
		const char* ptr = reinterpret_cast<char*>(_inputData.get()->GetData());
		auto size = _inputData->GetSize();

		Exec([&]()
		{
			_parsedElement = XmlParser::Parse(ptr, size);
		});
	}

	Size ParseXmlTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}
} // Elpida