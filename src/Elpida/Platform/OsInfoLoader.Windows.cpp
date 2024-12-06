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

		try
		{
			auto secondaryName = OsUtilities::ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "DisplayVersion");
			name += " " + secondaryName;
		}
		catch (const std::exception&)
		{
			// Older windows
			try
			{
				auto secondaryName = OsUtilities::ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "CSDVersion");
				name += " " + secondaryName;
			}
			catch (const std::exception&)
			{
				// give up on secondary
			}
		}

		String version;
		try
		{
			version = OsUtilities::ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "CurrentVersion");
		}
		catch (const std::exception&)
		{
			version = "Unknown version";
		}

		try
		{
			auto build = OsUtilities::ReadRegistryKeyFromHKLM(R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion)", "CurrentBuildNumber");
			version  += "." + build;
		}
		catch (const std::exception&)
		{
			// Its ok
		}

		return {
			"Windows",
			name,
			version
		};
	}
} // Elpida

#endif // defined(ELPIDA_WINDOWS)