//
// Created by klapeto on 30/11/2023.
//

#ifndef ELPIDA_INCLUDE_ELPIDA_CORE_STLALLOCATOR_HPP
#define ELPIDA_INCLUDE_ELPIDA_CORE_STLALLOCATOR_HPP

#include "Allocator.hpp"
#include "SharedPtr.hpp"
#include <cstdlib>
namespace Elpida
{

	template<typename T>
	class StlAllocator
	{
	public:
		template<typename U>
		friend class StlAllocator;

		typedef T value_type;
		explicit StlAllocator(SharedPtr<Allocator> allocator) noexcept
			: _allocator(std::move(allocator))
		{
		}

		template<typename U>
		explicit StlAllocator(const StlAllocator<U>& other) noexcept
			: _allocator(other._allocator)
		{

		}

		StlAllocator() = default;

		StlAllocator(StlAllocator&& other) noexcept
			: _allocator(std::move(other._allocator))
		{

		}

		StlAllocator(const StlAllocator& other) noexcept
			: _allocator(other._allocator)
		{

		}

		template<typename U>
		bool operator==(const StlAllocator<U>& other) const noexcept
		{
			return _allocator == other._allocator;
		}

		template<typename U>
		bool operator!=(const StlAllocator<U>& other) const noexcept
		{
			return _allocator != other._allocator;;
		}

		T* allocate(size_t n) const
		{
			if (n == 0)
			{
				return nullptr;
			}
			if (n > static_cast<size_t>(-1) / sizeof(T))
			{
				throw std::bad_array_new_length();
			}

			auto ptr = _allocator ? _allocator->Allocate(n * sizeof(T)) : malloc(n * sizeof(T));
			if (!ptr)
			{
				throw std::bad_alloc();
			}
			return static_cast<T*>(ptr);
		}

		void deallocate(T* ptr, size_t size) const noexcept
		{
			if (_allocator)
			{
				_allocator->Deallocate(ptr, size);
			}
			else
			{
				free(ptr);
			}
		}

	private:
		SharedPtr<Allocator> _allocator;
	};
}

#endif //ELPIDA_INCLUDE_ELPIDA_CORE_STLALLOCATOR_HPP
