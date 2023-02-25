/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2023  Ioannis Panagiotopoulos
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
// Created by klapeto on 25/2/2023.
//

#include "Elpida/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/SystemInfo/OsInfo.hpp"

#include <windows.h>
#include <strsafe.h>
#include "Elpida/Utilities/OsUtilities.hpp"

namespace Elpida
{

	void OsInfo::getOsInfoImpl()
	{
		try
		{
			_name = OsUtilities::ReadRegistryKeyFromHKLM("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");
		}
		catch (const std::exception&)
		{
			// does not exist?
			_name = "Unknown Windows";
		}

		try
		{
			_version = OsUtilities::ReadRegistryKeyFromHKLM("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ReleaseId");
		}
		catch (const std::exception&)
		{
			_version = "Uknown version";
		}

		try
		{
			_version  += "." + OsUtilities::ReadRegistryKeyFromHKLM("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuild");
		}
		catch (const std::exception&)
		{
			_version = "Uknown Build";
		}

		_category = "Windows";
	}
}

#endif