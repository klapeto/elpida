/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 14/4/20.
//

#include "BenchmarksController.hpp"

#include <filesystem>

#include <Elpida/Config.hpp>
#include <Elpida/ElpidaException.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/Utilities/Plugin/BenchmarksContainerPlugin.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/Configuration/Concrete/BenchmarkConfiguration.hpp>

#include "Models/GlobalConfigurationModel.hpp"

namespace Elpida
{

	BenchmarksController::BenchmarksController(ListModel<BenchmarkGroup>& model,
		AssociativeModel<std::string, BenchmarkConfiguration>& configurationsModel,
		const GlobalConfigurationModel& globalConfigurationModel,
		const ServiceProvider& serviceProvider,
		Logger& logger)
		: _logger(logger),
		  _model(model),
		  _configurationsModel(configurationsModel),
		  _globalConfigurationModel(globalConfigurationModel),
		  _serviceProvider(serviceProvider)
	{

	}

	BenchmarksController::~BenchmarksController()
	{
		destroyAll();
	}

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

	void BenchmarksController::destroyAll()
	{
		_configurationsModel.clear();
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
		_model.clear();
		_libraryLoader.unloadAll();
	}

	void BenchmarksController::reloadLibraries()
	{
		const auto& benchmarksPath = _globalConfigurationModel.getBenchmarksPath();
		try
		{
			_libraryLoader.unloadAll();

			for (auto& dir: std::filesystem::recursive_directory_iterator(benchmarksPath))
			{
				if (dir.is_directory()) continue;

				auto path = dir.path().string();
				if (hasLibraryExtension(path))
				{
					try
					{
						_libraryLoader.load(path);
					}
					catch (const std::exception& ex)
					{
						_logger.log(LogType::Error, "Failed to load Library:'" + path + "'", ex);
					}
				}
			}
		}
		catch (const std::exception& ex)
		{
			_logger.log(LogType::Error, "Failed to iterate Directory:'" + benchmarksPath + "'", ex);
		}
	}

	void BenchmarksController::reload()
	{
		destroyAll();
		reloadLibraries();
		const auto& loaded = _libraryLoader.getLoadedLibraries();
		for (const auto& lib: loaded)
		{
			auto versionFp = lib.second.getFunctionPointer<int32_t(*)()>("elpidaPluginAbiVersion");
			int32_t abiVersion = 0;
			if (versionFp)
			{
				abiVersion = versionFp();
			}
			else
			{
				continue;
			}

			auto factoryFp = lib.second
				.getFunctionPointer<BenchmarksContainerPlugin<Elpida::Benchmark>::Factory>("elpidaCreatePlugin");
			if (factoryFp != nullptr)
			{
				BenchmarksContainerPlugin<Elpida::Benchmark>* pPlugin = nullptr;
				try
				{
					pPlugin = factoryFp(&_serviceProvider);
				}
				catch (const std::exception& ex)
				{
					_logger.log(LogType::Error,
						Vu::concatenateToString("Plugin with name: '",
							lib.first,
							"' failed to produce task batches"),
						ex);
					throw;
				}

				if (pPlugin == nullptr)
				{
					_logger.log(LogType::Error, Vu::Cs("'", lib.first, "' plugin did not return any data!"));
					continue;
				}
				const auto& benchmarks = pPlugin->getUnderlyingData();
				_createdPlugins.emplace_back(lib.second, pPlugin);

				for (const auto& benchmark : benchmarks)
				{
					BenchmarkConfiguration configuration(*benchmark);
					for (const auto& builder : benchmark->getTaskBuilders())
					{
						configuration
							.addConfiguration(builder, builder.getDefaultConfiguration());
					}
					_configurationsModel.add(benchmark->getUuid(), std::move(configuration));
				}
				_model.add(BenchmarkGroup(pPlugin->getGroupName(), lib.first, benchmarks));
			}
		}
	}
}