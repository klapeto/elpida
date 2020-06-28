/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

//
// Created by klapeto on 26/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_DATA_ACTIVETASKDATA_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_DATA_ACTIVETASKDATA_HPP

#include "Elpida/Utilities/RawData.hpp"
#include "Elpida/Utilities/NumaMemory.hpp"

namespace Elpida
{
	class ActiveTaskData : public RawData
	{
	public:

		[[nodiscard]] pData getData() const override
		{
			return _memory.getPointer();
		}

		[[nodiscard]] size_t getSize() const override
		{
			return _memory.getSize();
		}

		ActiveTaskData(size_t size, const ProcessorNode& processor)
			: _memory(size, processor)
		{
			_memory.allocate();
		}
		~ActiveTaskData() override = default;
	private:
		NumaMemory _memory;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_DATA_ACTIVETASKDATA_HPP
