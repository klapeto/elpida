//
// Created by klapeto on 11/12/2022.
//

#include "BenchmarkLoader.hpp"

#include <Elpida/Config.hpp>
#include <Elpida/ElpidaException.hpp>
#include <Elpida/SharedLibraryLoader.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Utilities/Plugin/BenchmarksContainerPlugin.hpp>
#include <Elpida/Engine/Configuration/Concrete/BenchmarkConfiguration.hpp>

namespace Elpida
{
	static bool hasLibraryExtension(const std::string& path)
	{
		if (!path.empty())
		{
			auto ext = std::string(SharedLibraryLoader::LibrariesExtension);
			if (!ext.empty())
			{
				const auto pSize = path.size();
				const auto eSize = ext.size();

				if (pSize <= eSize) return false;    // path is less than extension or it is the extension

				for (auto pi = pSize - 1, ei = eSize - 1; ei > 0; --pi, --ei)
				{
					if (ext[ei] == path[pi]) continue;
					return false;
				}
				return true;
			}
			else
			{
				// wtf?
				throw ElpidaException(FUNCTION_NAME, "Library extension has no value. Probably a corrupt build or bug");
			}
		}
		return false;
	}

	const std::vector<std::unique_ptr<Benchmark>>& BenchmarkLoader::load(const std::string& path)
	{
		if (hasLibraryExtension(path))
		{
			auto library = _libraryLoader.getLoadedLibraries().find(path);
			if (library != _libraryLoader.getLoadedLibraries().end())
			{
				return load(library->second);
			}
			else
			{
				return load(_libraryLoader.load(path));
			}
		}
	}

	const std::vector<std::unique_ptr<Benchmark>>& BenchmarkLoader::load(const SharedLibrary& library)
	{
		auto versionFp = library.getFunctionPointer<int32_t(*)()>("elpidaPluginAbiVersion");

		int32_t abiVersion = 0;
		if (versionFp)
		{
			abiVersion = versionFp();
		}

		auto factoryFp = library
				.getFunctionPointer<BenchmarksContainerPlugin<Elpida::Benchmark>::Factory>("elpidaCreatePlugin");
		if (factoryFp != nullptr)
		{
			BenchmarksContainerPlugin<Elpida::Benchmark>* pPlugin = factoryFp(&_serviceProvider);
			if (pPlugin == nullptr)
			{
				throw ElpidaException(Vu::Cs("'", library.getPath(), "' plugin did not return any data!"));
			}

			_createdPlugins.emplace_back(library, pPlugin);
			return pPlugin->getUnderlyingData();
		}
	}

	BenchmarkLoader::~BenchmarkLoader()
	{
		for (auto&[library, plugin] : _createdPlugins)
		{
			auto destroyFunc =
					library.get().getFunctionPointer < void(*)
			(BenchmarksContainerPlugin<Elpida::Benchmark>*) > ("elpidaDestroyPlugin");
			if (destroyFunc)
			{
				destroyFunc(plugin);
			}
		}
		_createdPlugins.clear();
		_libraryLoader.unloadAll();
	}
} // Elpida