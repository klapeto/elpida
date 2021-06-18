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
// Created by klapeto on 27/10/20.
//

#ifndef INCLUDE_ELPIDA_TOPOLOGY_CPUCACHE_HPP
#define INCLUDE_ELPIDA_TOPOLOGY_CPUCACHE_HPP

#include <string>
#include <utility>

namespace Elpida
{
	class CpuCache final
	{
	public:

		[[nodiscard]]
		const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]]
		const std::string& getAssociativity() const
		{
			return _associativity;
		}

		[[nodiscard]]
		size_t getSize() const
		{
			return _size;
		}

		[[nodiscard]]
		size_t getLineSize() const
		{
			return _lineSize;
		}

		[[nodiscard]]
		size_t getLevel() const
		{
			return _level;
		}

		CpuCache(unsigned level,
			std::string name,
			std::string associativity,
			size_t size,
			size_t lineSize)
			: _name(std::move(name)), _associativity(std::move(associativity)), _level(level), _size(size),
			  _lineSize(lineSize)
		{
		}
		~CpuCache() = default;
	private:
		std::string _name;
		std::string _associativity;
		size_t _level;
		size_t _size;
		size_t _lineSize;
	};
}

#endif //INCLUDE_ELPIDA_TOPOLOGY_CPUCACHE_HPP
