/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
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
// Created by klapeto on 6/1/21.
//

#ifndef SRC_ELPIDA_SYSTEMINFO_CPUKIND_HPP
#define SRC_ELPIDA_SYSTEMINFO_CPUKIND_HPP

#include <string>
#include <unordered_map>

namespace Elpida
{
	class CpuKind
	{
	public:
		const std::unordered_map<std::string, std::string>& getInfos() const
		{
			return _infos;
		}

		int getEfficiency() const
		{
			return _efficiency;
		}

		CpuKind(int efficiency, std::unordered_map<std::string, std::string>&& infos)
		: _infos(std::move(infos)), _efficiency(efficiency)
		{

		}

		~CpuKind() = default;
	private:
		std::unordered_map<std::string, std::string> _infos;
		int _efficiency;
	};
}

#endif //SRC_ELPIDA_SYSTEMINFO_CPUKIND_HPP
