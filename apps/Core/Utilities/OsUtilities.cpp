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
// Created by klapeto on 13/9/20.
//

#include "OsUtilities.hpp"

#include <Elpida/Config.hpp>
#include <Elpida/ElpidaException.hpp>
#include <string>

#ifdef ELPIDA_LINUX
#include "xdg-open.h"
#else
#include <windows.h>
#include <Elpida/Utilities/ValueUtilities.hpp>
#endif

namespace Elpida
{

	void OsUtilities::openUrl(const std::string& url)
	{
#ifdef ELPIDA_LINUX
		auto res = xdg_open(url.c_str());
		if (res)
		{
			throw ElpidaException("Failed to open url", "Status code: " + std::to_string(res));
		}
#else
		auto res = ShellExecuteW(NULL, L"open", Vu::stringToWstring(url).c_str(), NULL, NULL, SW_SHOWDEFAULT);
		if (res <= (HINSTANCE)32)
		{
			std::string error;
			switch ((intptr_t)res)
			{
			case 0:
				error = "The operating system is out of memory or resources.";
				break;
			case ERROR_FILE_NOT_FOUND:
				error = "The specified file was not found.";
				break;
			case ERROR_PATH_NOT_FOUND:
				error = "The specified path was not found.";
				break;
			case ERROR_BAD_FORMAT:
				error = "The .exe file is invalid (non-Win32 .exe or error in .exe image).";
				break;
			case SE_ERR_ACCESSDENIED:
				error = "The operating system denied access to the specified file.";
				break;
			case SE_ERR_ASSOCINCOMPLETE:
				error = "The file name association is incomplete or invalid.";
				break;
			case SE_ERR_DDEBUSY:
				error =
					"The DDE transaction could not be completed because other DDE transactions were being processed.";
				break;
			case SE_ERR_DDEFAIL:
				error = "The DDE transaction failed.";
				break;
			case SE_ERR_DDETIMEOUT:
				error = "The DDE transaction could not be completed because the request timed out.";
				break;
			case SE_ERR_DLLNOTFOUND:
				error = "The specified DLL was not found.";
				break;
			case SE_ERR_NOASSOC:
				error =
					"There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable.";
				break;
			case SE_ERR_OOM:
				error = "There was not enough memory to complete the operation.";
				break;
			case SE_ERR_SHARE:
				error = "A sharing violation occurred. ";
				break;
			default:
				error = "Unknown Error";
				break;
			}

			throw ElpidaException("Failed to open url", error);
		}
#endif
	}
}