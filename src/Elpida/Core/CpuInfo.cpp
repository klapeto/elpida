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

#include "Elpida/Core/CpuInfo.hpp"

#include <utility>

namespace Elpida
{
	CpuInfo::CpuInfo(String architecture, String vendorName, String modelName)
		: _architecture(std::move(architecture)), _vendorName(std::move(vendorName)), _modelName(std::move(modelName))
	{
	}

	const String& CpuInfo::GetArchitecture() const
	{
		return _architecture;
	}

	const String& CpuInfo::GetVendorName() const
	{
		return _vendorName;
	}

	const String& CpuInfo::GetModelName() const
	{
		return _modelName;
	}
} // Elpida