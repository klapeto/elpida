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

		if (!SetHandleInformation(readHandle, HANDLE_FLAG_INHERIT, 0))
		{
			throw ElpidaException("Failed to set handle information: ", OsUtilities::GetLastErrorString());
		}

		if (!SetHandleInformation(writeHandle, HANDLE_FLAG_INHERIT, 0))
		{
			throw ElpidaException("Failed to set handle information: ", OsUtilities::GetLastErrorString());
		}

		_readHandle = readHandle;
		_writeHandle = writeHandle;
	}

	std::size_t AnonymousPipe::Read(char* buffer, std::size_t size) const
	{
		DWORD bytes;

		if (!ReadFile(std::any_cast<HANDLE>(_readHandle), buffer, size, &bytes, NULL))
		{
			throw ElpidaException("Failed to read from pipe: ", OsUtilities::GetLastErrorString());
		}

		return bytes;
	}

	std::size_t AnonymousPipe::Write(char* buffer, std::size_t size) const
	{
		DWORD bytes;

		if (!WriteFile(std::any_cast<HANDLE>(_readHandle), buffer, size, &bytes, NULL))
		{
			throw ElpidaException("Failed to write to pipe: ", OsUtilities::GetLastErrorString());
		}

		return bytes;
	}

	void AnonymousPipe::Close()
	{
		if (_readHandle.has_value())
		{
			CloseHandle(GetReadHandle<HANDLE>());
		}

		if (_writeHandle.has_value())
		{
			CloseHandle(GetWriteHandle<HANDLE>());
		}
	}

	bool AnonymousPipe::IsOpen() const
	{
		return _readHandle.has_value();
	}
} // Elpida

#endif // defined(ELPIDA_WINDOWS)