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
 * MemoryFile.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_MEMORYFILE_HPP_
#define ELPIDA_UTILITIES_MEMORYFILE_HPP_

#include "Elpida/Types/String.hpp"
#include "Elpida/Types/RawData.hpp"
#include "Elpida/Types/Primitives.hpp"

namespace Elpida
{

	class MemoryFile
	{
		public:

			const RawDataPtr& getData() const
			{
				return _data;
			}

			const Size& getSize() const
			{
				return _size;
			}

			void load(const String& path);
			void writeToFile(const String& path) const;

			MemoryFile();
			MemoryFile(Size size);
			MemoryFile(void* data, Size size);
			virtual ~MemoryFile();

			MemoryFile(MemoryFile&&) = default;
			MemoryFile(const MemoryFile&) = default;
			MemoryFile& operator=(MemoryFile&&) = default;
			MemoryFile& operator=(const MemoryFile&) = default;
		private:
			RawDataPtr _data;
			Size _size;
			bool _deleteData;
	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_MEMORYFILE_HPP_ */
