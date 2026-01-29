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
// Created by klapeto on 6/5/2023.
//

#include "Elpida/Core/Config.hpp"
#include <cstddef>

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/AnonymousPipe.hpp"

#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <windows.h>

namespace Elpida
{
	void AnonymousPipe::Open()
	{
		SECURITY_ATTRIBUTES securityAttributes;
		securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
		securityAttributes.bInheritHandle = TRUE;
		securityAttributes.lpSecurityDescriptor = NULL;

		HANDLE readHandle = NULL;
		HANDLE writeHandle = NULL;

		if (!CreatePipe(&readHandle, &writeHandle, &securityAttributes, 0))
		{
			throw ElpidaException("Failed to create pipe: ", OsUtilities::GetLastErrorString());
		}

		_readHandle = readHandle;
		_writeHandle = writeHandle;
	}

	std::size_t AnonymousPipe::Read(char* buffer, std::size_t size) const
	{
		DWORD bytes;

		if (!ReadFile(GetReadHandle<HANDLE>(), buffer, size, &bytes, NULL))
		{
			throw ElpidaException("Failed to read from pipe: ", OsUtilities::GetLastErrorString());
		}

		return bytes;
	}

	std::size_t AnonymousPipe::Write(char* buffer, std::size_t size) const
	{
		DWORD bytes;

		if (!WriteFile(GetWriteHandle<HANDLE>(), buffer, size, &bytes, NULL))
		{
			throw ElpidaException("Failed to write to pipe: ", OsUtilities::GetLastErrorString());
		}

		return bytes;
	}

	void AnonymousPipe::CloseRead()
	{
		if (_readHandle.has_value())
		{
			CloseHandle(GetReadHandle<HANDLE>());
			_readHandle = {};
		}
	}

	void AnonymousPipe::CloseWrite()
	{
		if (_writeHandle.has_value())
		{
			CloseHandle(GetWriteHandle<HANDLE>());
			_writeHandle = {};
		}
	}

	bool AnonymousPipe::IsOpen() const
	{
		return _readHandle.has_value();
	}
} // Elpida

#endif // defined(ELPIDA_WINDOWS)