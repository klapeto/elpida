//
// Created by klapeto on 11/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/BenchmarkGroupModule.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <dlfcn.h>

namespace Elpida
{
	void* BenchmarkGroupModule::LoadModule(const String& filePath)
	{
		auto ptr = dlopen(filePath.c_str(), RTLD_LAZY);
		if (ptr == nullptr)
		{
			throw ElpidaException("Failed to load module: '", filePath, "': ", dlerror());
		}

		return ptr;
	}

	void* BenchmarkGroupModule::GetFunction(const char* name, void* module)
	{
		return dlsym(module, name);
	}

	void BenchmarkGroupModule::UnloadModule(void* module)
	{
		dlclose(module);
	}

} // Elpida


#endif // defined(ELPIDA_UNIX)