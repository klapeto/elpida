/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 31/7/2024.
//

#ifndef ELPIDA_MEMORYSTREAM_HPP
#define ELPIDA_MEMORYSTREAM_HPP

#include "Memory.hpp"
#include "DefaultAllocator.hpp"
#include <cstring>

namespace Elpida
{
	class MemoryStream final
	{
	public:

		template<typename T>
		void Write(T* data, std::size_t size)
		{
			EnsureCapacity(_size + size);

			std::memcpy(GetPtrOffset(_writePosition), data, size);
			_writePosition += size;
			_size += size;
		}

		template<typename T>
		std::size_t Read(T* data, std::size_t size)
		{
			auto bytesToRead = size;
			if (_readPosition + size > _size)
			{
				bytesToRead = _size - _readPosition;
			}
			if (bytesToRead == 0) return 0;

			std::memcpy(data, GetPtrOffset(_readPosition), bytesToRead);

			_readPosition += bytesToRead;
			return bytesToRead;
		}

		void Reset()
		{
			_readPosition = 0;
			_writePosition = 0;
			_size = 0;
		}

		[[nodiscard]]
		std::size_t GetSize() const
		{
			return _size;
		}

		[[nodiscard]]
		std::size_t GetCapacity() const
		{
			return _memory.GetSize();
		}

		[[nodiscard]]
		Memory Detach()
		{
			if (_size == 0) return Memory(_memory.GetAllocator());

			_memory.ReAllocate(_size);
			_writePosition = 0;
			_readPosition = 0;
			_size = 0;
			return std::move(_memory);
		}

		[[nodiscard]]
		SharedPtr<Allocator> GetAllocator() const
		{
			return _memory.GetAllocator();
		}

		void EnsureCapacity(std::size_t size)
		{
			auto memorySize = _memory.GetSize();

			if (memorySize > size) return;

			if (size < 256)
			{
				size = 256;
			}

			if (memorySize == 0)
			{
				_memory.Allocate(size);
				return;
			}

			auto targetSize = static_cast<std::size_t>(memorySize * CapacityIncreaseMultiplier);
			if (size > targetSize)
			{
				targetSize = static_cast<std::size_t>(size * CapacityIncreaseMultiplier);
			}

			_memory.ReAllocate(targetSize);
		}

		[[nodiscard]]
		bool Eof() const
		{
			return _readPosition >= _size;
		}

		explicit MemoryStream(Memory&& memory)
				:_memory(std::move(memory)), _writePosition(0), _readPosition(0), _size(_memory.GetSize())
		{

		}

		explicit MemoryStream(SharedPtr<Allocator> allocator)
				:_memory(std::move(allocator)), _writePosition(0), _readPosition(0), _size(0)
		{

		}

		MemoryStream(MemoryStream&& other)=default;
		MemoryStream& operator=(MemoryStream&& other) noexcept=default;
		~MemoryStream() = default;
	private:
		Memory _memory;
		std::size_t _writePosition;
		std::size_t _readPosition;
		std::size_t _size;

		static inline const double CapacityIncreaseMultiplier = 2.0;

		void* GetPtrOffset(std::size_t position)
		{
			return reinterpret_cast<void*>(reinterpret_cast<std::size_t>(_memory.GetData()) + position);
		}
	};

} // Elpida

#endif //ELPIDA_MEMORYSTREAM_HPP
