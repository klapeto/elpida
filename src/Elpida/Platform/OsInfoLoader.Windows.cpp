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
// Created by klapeto on 13/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <windows.h>

namespace Elpida
{
	OsInfo OsInfoLoader::Load()
	{
		String name;
		try
		{
			name = OsUtilities::ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "ProductName");
		}
		catch (const std::exception&)
		{
			// does not exist?
			name = "Unknown Windows";
		}

		try
		{
			auto secondaryName = OsUtilities::ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "DisplayVersion");
			name += " " + secondaryName;
		}
		catch (const std::exception&)
		{
			// Older windows
			try
			{
				auto secondaryName = OsUtilities::ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "CSDVersion");
				name += " " + secondaryName;
			}
			catch (const std::exception&)
			{
				// give up on secondary
			}
		}

		String version;
		try
		{
			version = OsUtilities::ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "CurrentVersion");
		}
		catch (const std::exception&)
		{
			version = "Unknown version";
		}

		try
		{
			auto build = OsUtilities::ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "CurrentBuildNumber");
			version  += "." + build;
		}
		catch (const std::exception&)
		{
			// Its ok
		}

		return {
			"Windows",
			name,
			version
		};
	}
} // Elpida

#endif // defined(ELPIDA_WINDOWS)