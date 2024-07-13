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

		String version;
		try
		{
			version = OsUtilities::ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "ReleaseId");
		}
		catch (const std::exception&)
		{
			version = "Unknown version";
		}

		try
		{
			version  += "." + OsUtilities::ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "CurrentBuild");
		}
		catch (const std::exception&)
		{
			version = "Unknown Build";
		}

		return {
			"Windows",
			name,
			version
		};
	}
} // Elpida

#endif // defined(ELPIDA_WINDOWS)