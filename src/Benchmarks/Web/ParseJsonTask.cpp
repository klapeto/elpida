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

#include "ParseJsonTask.hpp"
#include "Elpida/Core/SimpleTaskData.hpp"

namespace Elpida
{
	void ParseJsonTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> ParseJsonTask::Finalize()
	{
		return std::make_shared<SimpleTaskData<nlohmann::json>>(std::move(_parsedElement), _inputData->GetAllocator());
	}

	TaskInfo ParseJsonTask::DoGetInfo() const
	{
		return { "JSON Parsing",
				 "Parses an JSON document and measures the parsing throughput",
				 "chars",
				 "How many characters are processes in the time",
				 ResultType::Throughput
		};
	}

	Size ParseJsonTask::GetProcessedDataSize() const
	{
		return _inputData->GetSize();
	}

	void ParseJsonTask::DoRunImpl()
	{
		const char* ptr = reinterpret_cast<char*>(_inputData.get()->GetData());
		std::string_view str{ ptr, _inputData->GetSize() };

		Exec([&]()
		{
			_parsedElement = nlohmann::json::parse(str);
		});
	}

	Size ParseJsonTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> ParseJsonTask::DoDuplicate() const
	{
		return std::make_unique<ParseJsonTask>();
	}
} // Elpida