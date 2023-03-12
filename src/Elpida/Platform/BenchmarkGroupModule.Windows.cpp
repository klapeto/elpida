//
// Created by klapeto on 12/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/BenchmarkGroupModule.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <windows.h>
#include <strsafe.h>

namespace Elpida
{
	void* BenchmarkGroupModule::LoadModule(const String& filePath)
	{
		auto ptr = LoadLibrary(filePath.c_str());
		if (ptr == nullptr)
		{
			throw ElpidaException("Failed to load module: '", filePath, "': ", OsUtilities::GetLastErrorString());
		}

		return ptr;
	}

	void* BenchmarkGroupModule::GetFunction(const char* name, void* module)
	{
		return (void*)GetProcAddress((HMODULE)module, name);
	}

	void BenchmarkGroupModule::UnloadModule(void* module)
	{
		FreeLibrary((HMODULE)module);
	}

} // Elpida

#endif // defined(ELPIDA_WINDOWS)