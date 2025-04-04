//
// Created by klapeto on 31/7/2024.
//

#ifndef ELPIDA_MEMORY_HPP
#define ELPIDA_MEMORY_HPP

#include <cstddef>
#include "Elpida/Core/Allocator.hpp"
#include "Elpida/Core/SharedPtr.hpp"
#include "Elpida/Core/ElpidaException.hpp"

namespace Elpida
{
	class Memory final
	{
	public:
		void* GetData()
		{
			return _ptr;
		}

		template<typename T>
		T* GetData()
		{
			return reinterpret_cast<T*>(_ptr);
		}

		template<typename T>
		const T* GetData() const
		{
			return reinterpret_cast<const T*>(_ptr);
		}

		[[nodiscard]]
		std::size_t GetSize() const
		{
			return _size;
		}

		[[nodiscard]]
		SharedPtr<Allocator> GetAllocator() const
		{
			return _allocator;
		}

		[[nodiscard]]
		explicit operator bool() const
		{
			return _ptr != nullptr;
		}

		void Deallocate()
		{
			if (_ptr != nullptr)
			{
				_allocator->Deallocate(_ptr, _size);
				_size = 0;
			}
		}

		void Allocate(std::size_t size)
		{
			Deallocate();
			_ptr = _allocator->Allocate(size);
			if (_ptr == nullptr)
			{
				throw ElpidaException("Could not allocate ", size, " bytes");
			}
			_size = size;
		}

		void ReAllocate(std::size_t size)
		{
			auto thisPtr = _allocator->Reallocate(_ptr, _size, size);
			if (thisPtr == nullptr)
			{
				throw ElpidaException("Could not re allocate ", size, " bytes");
			}
			_ptr = thisPtr;
			_size = size;
		}

		[[nodiscard]]
		void* ReleaseOwnership()
		{
			auto ptr = _ptr;
			_ptr = nullptr;
			_size = 0;
			return ptr;
		}

		void Compact()
		{
			if (_size != 0)
			{
				ReAllocate(_size);
			}
		}

		explicit Memory(SharedPtr<Allocator> allocator)
				:_allocator(std::move(allocator)), _ptr(nullptr), _size(0)
		{

		}

		Memory(const Memory&) = delete;

		Memory(Memory&& other) noexcept
		{
			_ptr = other._ptr;
			_size = other._size;
			_allocator = other._allocator;
			other._ptr = nullptr;
			other._size = 0;
		}

		Memory& operator=(Memory&& other) noexcept
		{
			Deallocate();
			_ptr = other._ptr;
			_size = other._size;
			_allocator = other._allocator;
			other._ptr = nullptr;
			other._size = 0;
			return *this;
		}

		Memory& operator=(const Memory& other) = delete;

		~Memory()
		{
			Deallocate();
		}

	private:
		SharedPtr<Allocator> _allocator;
		void* _ptr;
		std::size_t _size;
	};

} // Elpida

#endif //ELPIDA_MEMORY_HPP
