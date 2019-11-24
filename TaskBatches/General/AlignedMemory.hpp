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
 * AlignedMemory.hpp
 *
 *  Created on: 21 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_GENERAL_ALIGNEDMEMORY_HPP_
#define TASKBATCHES_GENERAL_ALIGNEDMEMORY_HPP_

#include <cstddef>

#include "TaskBatches/General/Memory.hpp"

namespace Elpida
{

	class AlignedMemory final: public Memory
	{
		public:
			unsigned int getAlignment() const
			{
				return _alignment;
			}

			void setAlignment(unsigned int alignment)
			{
				_alignment = alignment;
			}

			AlignedMemory(std::size_t size, unsigned int alignment)
					: Memory(size), _alignment(alignment)
			{
			}

			~AlignedMemory()
			{
				deallocate();
			}
		private:
			unsigned int _alignment;
		protected:
			void allocateImpl() override;
			void deallocateImpl() override;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_GENERAL_ALIGNEDMEMORY_HPP_ */
