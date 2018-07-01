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

#ifndef SRC_UTILITIES_MEMORYFILE_HPP_
#define SRC_UTILITIES_MEMORYFILE_HPP_

#include <string>

namespace Elpida
{

	class MemoryFile
	{
		public:

			unsigned char* const& getData() const
			{
				return _data;
			}

			const size_t& getSize() const
			{
				return _size;
			}

			void load(const std::string& path);
			void writeToFile(const std::string& path) const;

			MemoryFile();
			MemoryFile(size_t size);
			MemoryFile(void* data, size_t size);
			virtual ~MemoryFile();

			MemoryFile(MemoryFile&&) = default;
			MemoryFile(const MemoryFile&) = default;
			MemoryFile& operator=(MemoryFile&&) = default;
			MemoryFile& operator=(const MemoryFile&) = default;
		private:
			unsigned char* _data;
			size_t _size;
			bool _deleteData;
	};

} /* namespace Elpida */

#endif /* SRC_UTILITIES_MEMORYFILE_HPP_ */
