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

#ifndef ELPIDA_COMMONTASKS_MEMORY_HPP_
#define ELPIDA_COMMONTASKS_MEMORY_HPP_

#include <cstddef>
#include <mutex>

#include "Elpida/Utilities/NonCopyable.hpp"

namespace Elpida
{

	class Memory : public NonCopyable
	{
	public:
		using Data = unsigned char;
		using pData = Data*;

		[[nodiscard]] pData getPointer() const
		{
			return _pointer;
		}

		[[nodiscard]] std::size_t getSize() const
		{
			return _size;
		}

		void allocate();
		void deallocate();

		explicit Memory(std::size_t size);
		Memory(Memory&& other) noexcept;
		~Memory() override;
	private:
		std::mutex _mutex;
	protected:
		std::size_t _size;
		pData _pointer;

		virtual void allocateImpl() = 0;
		virtual void deallocateImpl() = 0;
	};

} /* namespace Elpida */

#endif /* ELPIDA_COMMONTASKS_MEMORY_HPP_ */
