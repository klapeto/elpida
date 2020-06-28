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
 * MemoryFile.hpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_MEMORYFILE_HPP_
#define ELPIDA_UTILITIES_MEMORYFILE_HPP_

#include <string>

namespace Elpida
{

	class Memory;
	class ProcessorNode;

	class MemoryFile
	{
	public:

		using Data = unsigned char;
		using pData = Data*;

		[[nodiscard]] const pData& getData() const
		{
			return _data;
		}

		[[nodiscard]] const std::size_t& getSize() const
		{
			return _size;
		}

		void load(const std::string& path);
		void writeToFile(const std::string& path) const;

		explicit MemoryFile(const ProcessorNode& processor);
		MemoryFile(void* data, std::size_t size);
		virtual ~MemoryFile();

		MemoryFile(MemoryFile&&) = default;
		MemoryFile(const MemoryFile&) = delete;
		MemoryFile& operator=(MemoryFile&&) = default;
		MemoryFile& operator=(const MemoryFile&) = delete;
	private:
		pData _data;
		Memory* _allocatedMemory;
		std::size_t _size;
		const ProcessorNode* _processor;
		bool _deleteData;

		void allocateData();
		void destroyData();
	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_MEMORYFILE_HPP_ */
