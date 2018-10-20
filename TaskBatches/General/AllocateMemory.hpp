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
 * AllocateMemory.hpp
 *
 *  Created on: 20 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_GENERAL_ALLOCATEMEMORY_HPP_
#define TASKBATCHES_GENERAL_ALLOCATEMEMORY_HPP_

#include <Elpida/Task.hpp>
#include <Elpida/Types/Primitives.hpp>

namespace Elpida
{

	class AllocateMemory final: public Task
	{
		public:

			Size getSize() const
			{
				return _size;
			}

			void*& getData()
			{
				return _data;
			}

			unsigned int getAlignment() const
			{
				return _alignment;
			}

			void setAlignment(unsigned int alignment)
			{
				_alignment = alignment;
			}

			void setSize(Size size)
			{
				_size = size;
			}

			void prepare() override
			{
				deallocate();
			}

			void finalize() override;

			void run() override;
			void calculateResults() override;

			AllocateMemory(Size size, bool initialize = false, int alignment = -1);
			~AllocateMemory();

		private:
			TaskRunResult _result;
			Size _size;
			void* _data;
			int _alignment;
			bool _initialize;

			void deallocate();
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_GENERAL_ALLOCATEMEMORY_HPP_ */
