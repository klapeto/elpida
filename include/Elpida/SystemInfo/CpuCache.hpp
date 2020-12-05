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

		[[nodiscard]] const std::string& getName() const
		{
			return _name;
		}
		[[nodiscard]] const std::string& getAssociativity() const
		{
			return _associativity;
		}
		[[nodiscard]] unsigned getSize() const
		{
			return _size;
		}

		[[nodiscard]] unsigned getLinesPerTag() const
		{
			return _linesPerTag;
		}

		[[nodiscard]] unsigned getLineSize() const
		{
			return _lineSize;
		}

		[[nodiscard]] unsigned int getLevel() const
		{
			return _level;
		}

		CpuCache(unsigned level,
			std::string name,
			std::string associativity,
			unsigned size,
			unsigned linesPerTag,
			unsigned lineSize)
			: _name(std::move(name)), _associativity(std::move(associativity)), _level(level), _size(size),
			  _linesPerTag(linesPerTag),
			  _lineSize(lineSize)
		{
		}
		~CpuCache() = default;
	private:
		std::string _name;
		std::string _associativity;
		unsigned _level;
		unsigned _size;
		unsigned _linesPerTag;
		unsigned _lineSize;
	};
}

#endif //INCLUDE_ELPIDA_TOPOLOGY_CPUCACHE_HPP
