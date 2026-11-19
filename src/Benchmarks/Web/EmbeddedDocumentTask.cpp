/*
 *  Copyright (c) 2026  Ioannis Panagiotopoulos
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

#include "EmbeddedDocumentTask.hpp"
#include "Elpida/Core/RawTaskData.hpp"
#include <cstring>

namespace Elpida::Benchmarks::Web
{
	EmbeddedDocumentTask::EmbeddedDocumentTask(const unsigned char* data, std::size_t size)
			: _data(data), _size(size)
	{
	}

	void EmbeddedDocumentTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_outputData = std::make_unique<RawTaskData>(inputData->GetAllocator());
		_outputData->Allocate(_size);
	}

	SharedPtr<AbstractTaskData> EmbeddedDocumentTask::Finalize()
	{
		return std::move(_outputData);
	}

	Size EmbeddedDocumentTask::GetProcessedDataSize() const
	{
		return _size;
	}

	void EmbeddedDocumentTask::DoRun()
	{
		std::memcpy(_outputData->GetData(), _data, _size);
	}

	UniquePtr<Task> EmbeddedDocumentTask::DoDuplicate() const
	{
		return std::make_unique<EmbeddedDocumentTask>(_data, _size);
	}

	TaskInfo EmbeddedDocumentTask::DoGetInfo() const
	{
		return {
				"Document loading",
				"Copies the built-in document",
				"B",
				"How many bytes are copied",
				ResultType::Throughput
		};
	}
} // Elpida
