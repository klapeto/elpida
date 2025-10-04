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
// Created by klapeto on 9/6/2024.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS) && (defined(_M_ARM64) || defined(__aarch64__))

#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Platform/OsUtilities.hpp"

namespace Elpida{

	CpuInfo CpuInfoLoader::Load()
	{
		std::string vendor;
		try
		{
			vendor = OsUtilities::ReadRegistryKeyFromHKLM(R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", "VendorIdentifier");
		}
		catch (const std::exception&)
		{
			vendor = "Unknown Vendor";
		}

		std::string modelName;
		try
		{
			modelName = OsUtilities::ReadRegistryKeyFromHKLM(R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", "ProcessorNameString");
		}
		catch (const std::exception&)
		{
			modelName = "Unknown Model";
		}

		return CpuInfo("aarch64", std::move(vendor), std::move(modelName));
	}
}

#endif