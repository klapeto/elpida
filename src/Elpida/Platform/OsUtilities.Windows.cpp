//
// Created by klapeto on 12/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/OsUtilities.hpp"

#include <windows.h>
#include <strsafe.h>

namespace Elpida
{

	unsigned int OsUtilities::GetNumaNodeIdForProcessor(unsigned int processorId)
	{
		UCHAR NodeNumber;

		GetNumaProcessorNode((UCHAR)processorId, &NodeNumber);	// TODO: does not work with 64+ processors (thanks windows)
		return NodeNumber;
	}

	String OsUtilities::GetLastErrorString()
	{
		HRESULT errorId = GetLastError();

		if (errorId == 0) return String();

		LPSTR messageBuffer = nullptr;
		try
		{
			size_t size = FormatMessageA(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, errorId, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPSTR) & messageBuffer, 0, NULL);

			String message(messageBuffer, size);

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

} // Elpida

#endif // defined(ELPIDA_WINDOWS)