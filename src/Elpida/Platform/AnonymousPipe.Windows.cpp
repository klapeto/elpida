//
// Created by klapeto on 6/5/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/AnonymousPipe.hpp"

#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <windows.h>

namespace Elpida
{
	AnonymousPipe::AnonymousPipe()
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
} // Elpida

#endif // defined(ELPIDA_WINDOWS)