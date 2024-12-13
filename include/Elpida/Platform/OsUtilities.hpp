//
// Created by klapeto on 12/3/2023.
//

#ifndef ELPIDA_OSUTILITIES_HPP_
#define ELPIDA_OSUTILITIES_HPP_

#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/String.hpp"
#include <filesystem>

#if defined(ELPIDA_WINDOWS)
#include <Windows.h>
#endif

namespace Elpida
{

	class OsUtilities
	{
	 public:
		static String GetLastErrorString();
		static std::filesystem::path GetExecutableDirectory();
		static std::filesystem::path GetExecutablePath();
		static unsigned int GetNumaNodeIdForProcessor(unsigned int processorId);

		static bool ConvertArgumentsToUTF8(int& originalArgC, char**& originalArgV);

#if defined(ELPIDA_WINDOWS)
		static std::string GetStringFromWinApiBuffer(LPCTSTR buffer, DWORD size);

		static
#if UNICODE
		std::wstring
#else
		std::string
#endif
		GetWinApiStringFromString(const std::string& str);

		static std::string ReadRegistryKeyFromHKLM(const std::string& subKey, const std::string& key);
#endif
	};

} // Elpida

#endif //ELPIDA_OSUTILITIES_HPP_
