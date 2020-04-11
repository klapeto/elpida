//
// Created by klapeto on 11/4/20.
//

#include "Elpida/Utilities/WindowsUtils.hpp"

#ifdef ELPIDA_WINDOWS
#include <Windows.h>
#include <strsafe.h>
#endif
namespace Elpida
{

#ifdef ELPIDA_WINDOWS
	std::string WindowsUtils::GetLastErrorString()
	{
		try
		{
			DWORD errorMessageID = GetLastError();
			if (errorMessageID == 0) return std::string();

			LPSTR messageBuffer = nullptr;
			size_t size = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPSTR) & messageBuffer, 0, NULL);

			std::string message(messageBuffer, size);

			LocalFree(messageBuffer);
			return message;
		}
		catch (...)
		{
			if (messageBuffer != nullptr)
			{
				LocalFree(messageBuffer);
			}

			throw;
		}

	}
#endif
}
