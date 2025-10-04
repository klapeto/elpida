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

#ifndef ELPIDA_ALLOCATOR_HPP_
#define ELPIDA_ALLOCATOR_HPP_

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/Size.hpp"

namespace Elpida
{
	class ProcessingUnitNode;

	class Allocator
	{
	public:
		[[nodiscard]]
		virtual void* Allocate(Size size) = 0;
		virtual void Deallocate(void* ptr, Size size) noexcept = 0;
		virtual void* Reallocate(void* ptr, Size oldSize, Size newSize) = 0;

		[[nodiscard]]
		Duration GetTotalTime() const
		{
			return _totalTime;
		}

		[[nodiscard]]
		Size GetAllocations() const
		{
			return _totalAllocations;
		}
		void ResetStatistics()
		{
			_totalTime = Seconds(0);
			_totalAllocations = 0;
		}

		Allocator() = default;
		virtual ~Allocator() = default;
	protected:
		Duration _totalTime{0};
		Size _totalAllocations{0};
	};

} // Elpida

#endif //ELPIDA_ALLOCATOR_HPP_
