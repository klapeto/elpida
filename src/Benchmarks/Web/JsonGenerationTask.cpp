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
// Created by klapeto on 21/5/2024.
//

#include <cmath>
#include <cstring>
#include "JsonGenerationTask.hpp"

namespace Elpida
{
	static const char Header[] = R"([)";
	static const char Element[] = R"({"test":"data","obj":{"a":true,"b":"random"},"arr":["a","b","c"]},)";
	static const char Footer[] = R"(])";

	JsonGenerationTask::JsonGenerationTask(std::size_t jsonSize)
			:_elementCount(1), _jsonSize(jsonSize)
	{
	}

	void JsonGenerationTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_outputData = std::make_unique<RawTaskData>(inputData->GetAllocator());

		_elementCount = std::max(1.0, std::ceil(_jsonSize / static_cast<double>(sizeof(Element))));
		_jsonSize = sizeof(Header) + _elementCount * sizeof(Element) + sizeof(Footer) - 1;
		_outputData->Allocate(_jsonSize);
	}

	SharedPtr<AbstractTaskData> JsonGenerationTask::Finalize()
	{
		return std::move(_outputData);
	}

	Size JsonGenerationTask::GetProcessedDataSize() const
	{
		return _jsonSize;
	}

	void JsonGenerationTask::DoRun()
	{
		auto ptr = _outputData->GetData();
		std::memcpy(ptr, Header, sizeof(Header) - 1);
		ptr += sizeof(Header) - 1;

		for (std::size_t i = 0; i < _elementCount; ++i)
		{
			std::memcpy(ptr, Element, sizeof(Element) - 1);
			ptr += sizeof(Element) - 1;
		}

		std::memcpy(ptr - 1, Footer, sizeof(Footer) - 1);

	}

	TaskInfo JsonGenerationTask::DoGetInfo() const
	{
		return TaskInfo("Json generation",
				"Generates a json file in memory with the provided size",
				"B",
				"The rate of generation",
				ResultType::Throughput);
	}

	UniquePtr<Task> JsonGenerationTask::DoDuplicate() const
	{
		return std::make_unique<JsonGenerationTask>(_jsonSize);
	}

} // Elpida