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
 * MemoryInfo.hpp
 *
 *  Created on: 21 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_MEMORYINFO_HPP_
#define ELPIDA_MEMORYINFO_HPP_

#include "Elpida/Utilities/NonCopyable.hpp"
#include "Elpida/Types/Primitives.hpp"

namespace Elpida
{

	class MemoryInfo final: public NonCopyable
	{
		public:

			static MemoryInfo& getInfo()
			{
				static MemoryInfo instance;
				return instance;
			}

			Size getMemorySize() const
			{
				return _memorySize;
			}

			Size getPageSize() const
			{
				return _pageSize;
			}

			Size getAvailableFreeMemory() const;

			~MemoryInfo()
			{
			}
		private:

			Size _memorySize;
			Size _pageSize;

			MemoryInfo()
					: _memorySize(0), _pageSize(0)
			{
				getValues();
			}

			void getValues();
	};

} /* namespace Elpida */

#endif /* ELPIDA_MEMORYINFO_HPP_ */