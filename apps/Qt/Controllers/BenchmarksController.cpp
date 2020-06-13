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

#include <Elpida/ElpidaException.hpp>
#include <Elpida/Utilities/FileSystem.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/Utilities/Plugin/BenchmarksContainerPlugin.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/Configuration/Specification/ConfigurationSpecificationBase.hpp>
#include <Elpida/Engine/Task/TaskBuilder.hpp>
#include <Elpida/Engine/Configuration/Concrete/BenchmarkConfiguration.hpp>

namespace Elpida
{

	BenchmarksController::BenchmarksController(ListModel<Benchmark*>& model,
		AssociativeModel<std::string, BenchmarkConfiguration>& configurationsModel,
		Logger& logger)
		: _logger(logger), _model(model), _configurationsModel(configurationsModel)
	{
#if ELPIDA_DEBUG_BUILD
		_benchmarksPath = TASK_BATCH_DEBUG_DIR;
#else
		_benchmarksPath = "./Benchmarks";	// TODO: Think of something more portable
#endif
	}

	BenchmarksController::~BenchmarksController()
	{
		destroyAll();
	}

	static bool hasLibraryExtension(const std::string& path)
	{
		if (!path.empty())
		{
			auto& ext = SharedLibraryLoader::LibrariesExtension;
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
		for (auto plugin: _createdPlugins)
		{
			delete plugin;
		}
		_createdPlugins.clear();
		_model.clear();
		_libraryLoader.unloadAll();
	}

	void BenchmarksController::reloadLibraries()
	{
		try
		{
			_libraryLoader.unloadAll();
			FileSystem::iterateDirectory(_benchmarksPath, [this](const std::string& path)
			{
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
			});
		}
		catch (const std::exception& ex)
		{
			_logger.log(LogType::Error, "Failed to iterate Directory:'" + _benchmarksPath + "'", ex);
		}
	}

	void BenchmarksController::reload()
	{
		destroyAll();
		reloadLibraries();
		const auto& loaded = _libraryLoader.getLoadedLibraries();
		for (const auto& lib: loaded)
		{
			auto factoryFp = lib.second
				.getFunctionPointer<BenchmarksContainerPlugin<Elpida::Benchmark>::Factory>("createPlugin");
			if (factoryFp != nullptr)
			{
				BenchmarksContainerPlugin<Elpida::Benchmark>* pPlugin = nullptr;
				try
				{
					pPlugin = factoryFp();
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
				_createdPlugins.push_back(pPlugin);
				for (auto benchmark : benchmarks)
				{
					BenchmarkConfiguration configuration(*benchmark);
					for (const auto& builder : benchmark->getTaskBuilders())
					{
						configuration
							.addConfiguration(builder->getTaskSpecification(), builder->getDefaultConfiguration());
					}
					_configurationsModel.add(benchmark->getId(), std::move(configuration));
					_model.add(benchmark);
				}
			}
		}
	}
}