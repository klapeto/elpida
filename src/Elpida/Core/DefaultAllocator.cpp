//
// Created by klapeto on 11/3/2023.
//

#include "Elpida/Core/DefaultAllocator.hpp"
#include "Elpida/Core/Duration.hpp"
#include <cstdlib>

namespace Elpida
{
	void* DefaultAllocator::Allocate(Size size)
	{
		auto a = Timer::now();
#if false
		auto ptr = _aligned_malloc(16, size);
#else
		auto ptr = malloc(size);
#endif
		//auto ptr = malloc(size);
		_totalAllocations++;
		auto b = Timer::now();
		_totalTime += b - a;
		return ptr;
	}

	void DefaultAllocator::Deallocate(void* ptr, Size size) noexcept
	{
		auto a = Timer::now();
		free(ptr);
		auto b = Timer::now();
		_totalTime += b - a;
	}

	void* DefaultAllocator::Reallocate(void* ptr, Size oldSize, Size newSize)
	{
		auto a = Timer::now();
		auto newPtr = realloc(ptr, newSize);
		auto b = Timer::now();
		_totalTime += b - a;
		return newPtr;
	}

} // Elpida