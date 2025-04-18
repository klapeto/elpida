//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 31/7/2024.
//

#include "ZlibDecompressionTask.hpp"

namespace Elpida
{
	static voidpf Allocate(voidpf opaque, uInt items, uInt size)
	{
		auto* allocator = static_cast<Allocator*>(opaque);

		auto totalSize = items * size;
		auto* ptr = allocator->Allocate(totalSize + sizeof(std::size_t));

		if (ptr == nullptr) return ptr;

		*static_cast<std::size_t*>(ptr) = totalSize;
		return reinterpret_cast<void*>(reinterpret_cast<std::size_t>(ptr) + sizeof(std::size_t));
	}

	static void Free(voidpf opaque, voidpf address)
	{
		auto* allocator = static_cast<Allocator*>(opaque);

		auto realAddress = reinterpret_cast<void*>(reinterpret_cast<std::size_t>(address) - sizeof(std::size_t));
		auto size = *reinterpret_cast<std::size_t*>(realAddress);

		allocator->Deallocate(realAddress, size);
	}

	void ZlibDecompressionTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_output.reset();
		_input = std::move(inputData);
		_output = MemoryStream(_input->GetAllocator());

		std::memset(&_zStream, 0, sizeof(z_stream));

		_zStream.zalloc = Allocate;
		_zStream.zfree = Free;
		_zStream.opaque = _output->GetAllocator().get();

		auto result = inflateInit(&_zStream);
		if (result != Z_OK)
		{
			throw ElpidaException("Failed to initialize zlib inflate: ", _zStream.msg);
		}

	}

	SharedPtr<AbstractTaskData> ZlibDecompressionTask::Finalize()
	{
		return std::make_shared<RawTaskData>(_output->Detach());
	}

	Size ZlibDecompressionTask::GetProcessedDataSize() const
	{
		return _input->GetSize();
	}

	void ZlibDecompressionTask::DoRunImpl()
	{
		auto input = _input->GetData();
		auto size = _input->GetSize();
		auto& output = _output.value();

		const int ChunkSize = 1024;

		Exec([&]()
		{
			output.Reset();

			inflateReset(&_zStream);
			_zStream.next_in = reinterpret_cast<Bytef*>(input);
			_zStream.avail_in = size;

			unsigned char buffer[ChunkSize];

			do
			{
				_zStream.next_out = buffer;
				_zStream.avail_out = ChunkSize;
				auto result = inflate(&_zStream, Z_FINISH);

				if (result == Z_STREAM_ERROR)
				{
					throw ElpidaException("Failed to inflate zlib: ", _zStream.msg);
				}

				auto written = ChunkSize - _zStream.avail_out;

				output.Write(buffer, written);
			} while (_zStream.avail_out == 0);
		});
	}

	TaskInfo ZlibDecompressionTask::DoGetInfo() const
	{
		return TaskInfo("Zlib Decompression",
				"Decompresses data with Zlib",
				"B",
				"The rate of decompression",
				ResultType::Throughput);
	}

	Size ZlibDecompressionTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> ZlibDecompressionTask::DoDuplicate() const
	{
		return std::make_unique<ZlibDecompressionTask>();
	}

	ZlibDecompressionTask::~ZlibDecompressionTask()
	{
		if (_zStream.zalloc != nullptr)
		{
			inflateEnd(&_zStream);
		}
	}

	ZlibDecompressionTask::ZlibDecompressionTask()
	{
		std::memset(&_zStream, 0, sizeof(z_stream));
	}

} // Elpida