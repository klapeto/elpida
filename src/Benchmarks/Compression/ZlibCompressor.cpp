//
// Created by klapeto on 31/7/2024.
//

#include "ZlibCompressor.hpp"

#include "Elpida/Core/MemoryStream.hpp"
#include "zlib.h"
#include <cstring>

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

	void ZlibCompressor::Decompress(void* input,
			std::size_t inputSize, MemoryStream& memoryStream)
	{
		const int ChunkSize = 1024;

		z_stream zStream;
		std::memset(&zStream, 0, sizeof(z_stream));

		zStream.zalloc = Allocate;
		zStream.zfree = Free;
		zStream.opaque = memoryStream.GetAllocator().get();

		auto result = inflateInit(&zStream);
		if (result != Z_OK)
		{
			throw ElpidaException("Failed to initialize zlib inflate: ", zStream.msg);
		}

		try
		{
			zStream.next_in = reinterpret_cast<Bytef*>(input);
			zStream.avail_in = inputSize;

			unsigned char buffer[ChunkSize];

			do
			{
				zStream.next_out = buffer;
				zStream.avail_out = ChunkSize;
				result = inflate(&zStream, Z_FINISH);

				if (result == Z_STREAM_ERROR)
				{
					throw ElpidaException("Failed to inflate zlib: ", zStream.msg);
				}

				auto written = ChunkSize - zStream.avail_out;

				memoryStream.Write(buffer, written);
			} while (zStream.avail_out == 0);

			inflateEnd(&zStream);
		}
		catch (...)
		{
			inflateEnd(&zStream);
			throw;
		}
	}

	void ZlibCompressor::Compress(void* input, std::size_t inputSize, MemoryStream& memoryStream)
	{
		const int ChunkSize = 1024;

		z_stream zStream;
		std::memset(&zStream, 0, sizeof(z_stream));

		zStream.zalloc = Allocate;
		zStream.zfree = Free;
		zStream.opaque = memoryStream.GetAllocator().get();

		auto result = deflateInit(&zStream, Z_BEST_COMPRESSION);
		if (result != Z_OK)
		{
			throw ElpidaException("Failed to initialize zlib: ", zStream.msg);
		}

		try
		{
			zStream.next_in = reinterpret_cast<Bytef*>(input);
			zStream.avail_in = inputSize;

			unsigned char buffer[ChunkSize];

			do
			{
				zStream.next_out = buffer;
				zStream.avail_out = ChunkSize;
				result = deflate(&zStream, Z_FINISH);

				if (result == Z_STREAM_ERROR)
				{
					throw ElpidaException("Failed to deflate zlib: ", zStream.msg);
				}

				auto written = ChunkSize - zStream.avail_out;

				memoryStream.Write(buffer, written);
			} while (zStream.avail_out == 0);

			deflateEnd(&zStream);
		}
		catch (...)
		{
			deflateEnd(&zStream);
			throw;
		}
	}
} // Elpida