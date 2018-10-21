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
 * Memory.hpp
 *
 *  Created on: 21 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_GENERAL_MEMORY_HPP_
#define TASKBATCHES_GENERAL_MEMORY_HPP_

#include <Elpida/Types/Primitives.hpp>
#include <Elpida/Utilities/NonCopyable.hpp>
#include <mutex>

namespace Elpida
{

	class Memory: public NonCopyable
	{
		public:
			void* getPointer() const
			{
				return _pointer;
			}

			Size getSize() const
			{
				return _size;
			}

			void setSize(Size size)
			{
				_size = size;
			}

			void allocate()
			{
				std::unique_lock<std::mutex> lock(_mutex);
				if (_pointer != nullptr)
				{
					deallocateImpl();
					_pointer = nullptr;
				}
				allocateImpl();
			}
			void deallocate()
			{
				std::unique_lock<std::mutex> lock(_mutex);
				if (_pointer != nullptr)
				{
					deallocateImpl();
					_pointer = nullptr;
				}
			}

			Memory(Size size)
					: _size(size), _pointer(nullptr)
			{

			}

			virtual ~Memory()
			{

			}

			Memory(Memory&& other)
			{
				std::unique_lock<std::mutex> lock(other._mutex);
				this->_size = other._size;
				other._size = 0;
				this->_pointer = other._pointer;
				other._pointer = nullptr;
			}

		private:
			std::mutex _mutex;
		protected:
			Size _size;
			void* _pointer;

			virtual void allocateImpl() = 0;
			virtual void deallocateImpl() = 0;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_GENERAL_MEMORY_HPP_ */
