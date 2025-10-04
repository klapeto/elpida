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
		auto ptr = malloc(size);
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