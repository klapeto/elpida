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

/*
 * NumaMemory.hpp
 *
 *  Created on: 9 Ιουν 2019
 *      Author: klapeto
 */

#ifndef ELPIDA_COMMONTASKS_NUMAMEMORY_HPP_
#define ELPIDA_COMMONTASKS_NUMAMEMORY_HPP_

#include <cstddef>

#include "Elpida/Utilities/Memory.hpp"

namespace Elpida
{
	class ProcessorNode;

	class NumaMemory final : public Memory
	{
	public:
		NumaMemory(std::size_t size, int numaNode);
		~NumaMemory() override;
	private:
		int _numaNode;
	protected:
		void allocateImpl() override;
		void deallocateImpl() override;

		static void* allocateOnNumaNode(std::size_t size, int numaNode);
		static void* deallocateOnNumaNode(void* ptr, std::size_t size);
	};

} /* namespace Elpida */

#endif /* ELPIDA_COMMONTASKS_NUMAMEMORY_HPP_ */
