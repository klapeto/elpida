/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * UnalignedMemory.hpp
 *
 *  Created on: 21 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_COMMONTASKS_UNALIGNEDMEMORY_HPP_
#define ELPIDA_COMMONTASKS_UNALIGNEDMEMORY_HPP_

#include <cstddef>

#include "Memory.hpp"

namespace Elpida
{

	class UnalignedMemory final : public Memory
	{
	public:
		UnalignedMemory(std::size_t size)
			: Memory(size)
		{

		}
		~UnalignedMemory()
		{
			deallocate();
		}
	protected:
		void allocateImpl() override;
		void deallocateImpl() override;
	};

} /* namespace Elpida */

#endif /* ELPIDA_COMMONTASKS_UNALIGNEDMEMORY_HPP_ */
