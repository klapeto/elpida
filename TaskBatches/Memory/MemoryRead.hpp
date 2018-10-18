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
 * MemoryRead.hpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_MEMORYREAD_HPP_
#define TASKBATCHES_MEMORY_MEMORYREAD_HPP_

#include <Elpida/Task.hpp>
#include <Elpida/Types/Integer.hpp>
#include <Elpida/Types/Primitives.hpp>
#include <Elpida/TaskRunResult.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <type_traits>

namespace Elpida
{

	template<typename T>
	class MemoryRead final: public Task
	{
		public:

			static_assert(std::is_trivially_constructible<T>::value);

			volatile Int32*& getData() const
			{
				return _data;
			}

			Size getDataSize() const
			{
				return _dataSize;
			}

			void setDataSize(Size dataSize)
			{
				_dataSize = dataSize;
			}

			void run() override
			{
				for (Size i = 0; i < _dataSize; ++i)
				{
					volatile T dummy = _data[i];
				}
			}

			void calculateResults() override
			{
				_runResult.setMeasuredValue(_dataSize * sizeof(T));
				addResult(_runResult);
			}

			void prepare() override
			{
				_data = new T[_dataSize];
			}

			void finalize() override
			{
				if (_data != nullptr)
				{
					delete[] _data;
					_data = nullptr;
				}
			}

			MemoryRead(Size dataSize)
					:
					  Task("Read " + ValueUtilities::getValueScale(dataSize * sizeof(T)) + "B@" + std::to_string(sizeof(T)) + " Bytes/Read"),
					  _runResult("Memory Read Bandwidth", "Bytes"),
					  _dataSize(dataSize),
					  _data(nullptr)
			{

			}
			~MemoryRead()
			{
				finalize();
			}

		private:
			TaskRunResult _runResult;
			Size _dataSize;
			T* _data;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_MEMORYREAD_HPP_ */
