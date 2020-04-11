//
// Created by klapeto on 11/4/20.
//

#include "Elpida/Utilities/WindowsUtils.hpp"


namespace Elpida
{
#ifdef ELPIDA_WINDOWS
	std::string Elpida::WindowsUtils::GetLastErrorString()
{


		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);

		lpDisplayBuf = (LPVOID) LocalAlloc(LMEM_ZEROINIT,
				(lstrlen((LPCTSTR) lpMsgBuf) + lstrlen((LPCTSTR) "Error") + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR) lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"), "Error",
				dw, lpMsgBuf);
		std::string returnString((const char*)lpDisplayBuf);
		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);

		return returnString;

}
#endif
}
