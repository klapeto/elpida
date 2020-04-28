//
// Created by klapeto on 14/4/20.
//

#include <Elpida/ElpidaException.hpp>
#include <Elpida/Utilities/FileSystem.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/Utilities/Plugin/BenchmarksContainerPlugin.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/Configuration/TaskConfigurationSpecifications.hpp>
#include <Elpida/Engine/Configuration/ConfigurationSpecificationBase.hpp>
#include <Elpida/Engine/Configuration/TaskConfiguration.hpp>
#include "BenchmarksController.hpp"

namespace Elpida
{

	BenchmarksController::BenchmarksController(ListModel<Benchmark*>& model,
		AssociativeModel<std::string, BenchmarkConfiguration>& configurationsModel,
		Logger& logger)
		:
		_model(model), _configurationsModel(configurationsModel), _logger(logger)
	{
#if ELPIDA_DEBUG_BUILD
		_benchmarksPath = TASK_BATCH_DEBUG_DIR;
#else
		_taskBatchPath = "./TaskBatches";	// TODO: Think of something more portable
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
					for (const auto& taskConfigSpecs :benchmark->getConfigurationSpecifications())
					{
						auto& taskSpec = taskConfigSpecs.getTaskSpecification();
						TaskConfiguration taskConfiguration(taskSpec);
						for (auto configSpec : taskConfigSpecs.getConfigurationSpecifications())
						{
							taskConfiguration.setConfiguration(configSpec->getName(), *configSpec->createDefault());
						}
						configuration.addConfiguration(taskSpec, std::move(taskConfiguration));
					}
					_configurationsModel.add(benchmark->getId(), std::move(configuration));
					_model.add(benchmark);
				}
			}
		}
	}
}