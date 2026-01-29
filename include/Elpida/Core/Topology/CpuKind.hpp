/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 2/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_CPUKIND_HPP_
#define ELPIDA_TOPOLOGY_CPUKIND_HPP_

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Map.hpp"

namespace Elpida
{

	class CpuKind final 
	{
	 public:
		[[nodiscard]]
		const Map<String, String>& GetInfos() const;

		[[nodiscard]]
		int GetEfficiency() const;

		CpuKind(int efficiency, Map<String, String>&& infos);
		CpuKind(const CpuKind&) = delete;
		CpuKind(CpuKind&&) noexcept = default;
		CpuKind& operator=(const CpuKind&) = delete;
		CpuKind& operator=(CpuKind&&) noexcept = default;
		~CpuKind() = default;
	 private:
		Map<String, String> _infos;
		int _efficiency;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_CPUKIND_HPP_
