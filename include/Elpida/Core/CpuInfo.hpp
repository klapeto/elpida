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
// Created by klapeto on 14/3/2023.
//

#ifndef ELPIDA_CPUINFO_HPP_
#define ELPIDA_CPUINFO_HPP_

#include "Elpida/Core/String.hpp"

namespace Elpida
{

	class CpuInfo final
	{
	 public:
		[[nodiscard]]
		const String& GetArchitecture() const;

		[[nodiscard]]
		const String& GetVendorName() const;

		[[nodiscard]]
		const String& GetModelName() const;

		CpuInfo(String architecture, String vendorName, String modelName);
		CpuInfo(const CpuInfo&) = default;
		CpuInfo(CpuInfo&&) noexcept = default;
		CpuInfo& operator=(const CpuInfo&) = default;
		CpuInfo& operator=(CpuInfo&&) noexcept = default;
		~CpuInfo() = default;
	 private:
		String _architecture;
		String _vendorName;
		String _modelName;
	};

} // Elpida

#endif //ELPIDA_CPUINFO_HPP_
