//
// Created by klapeto on 14/4/20.
//

#include <Elpida/ElpidaException.hpp>
#include <Elpida/Utilities/FileSystem.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/Utilities/Plugin/TaskBatchesContainerPlugin.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <TaskBatches/QtTaskBatchWrapper.hpp>
#include "TaskBatchesController.hpp"

namespace Elpida
{

	TaskBatchesController::TaskBatchesController(CollectionModel<QtTaskBatchWrapper*>& model, Logger& logger)
		:
		_model(model), _logger(logger)
	{
#if ELPIDA_DEBUG_BUILD
		_taskBatchPath = TASK_BATCH_DEBUG_DIR;
#else
		_taskBatchPath = "./TaskBatches";	// TODO: Think of something more portable
#endif
	}

	TaskBatchesController::~TaskBatchesController()
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
				throw ElpidaException(__func__, "Library extension has no value. Probably a corrupt build or bug");
			}
		}
		return false;
	}

	void TaskBatchesController::destroyAll()
	{
		for (auto plugin: _createdPlugins)
		{
			delete plugin;
		}
		_createdPlugins.clear();
		_model.clear();
		_libraryLoader.unloadAll();
	}

	void TaskBatchesController::reloadLibraries()
	{
		try
		{
			_libraryLoader.unloadAll();
			FileSystem::iterateDirectory(_taskBatchPath, [this](const std::string& path)
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
			_logger.log(LogType::Error, "Failed to iterate Directory:'" + _taskBatchPath + "'", ex);
		}
	}

	void TaskBatchesController::reload()
	{
		destroyAll();
		reloadLibraries();
		const auto& loaded = _libraryLoader.getLoadedLibraries();
		for (const auto& lib: loaded)
		{
			auto factoryFp = lib.second
				.getFunctionPointer<TaskBatchesContainerPlugin<Elpida::QtTaskBatchWrapper>::Factory>("createPlugin");
			if (factoryFp != nullptr)
			{
				TaskBatchesContainerPlugin<Elpida::QtTaskBatchWrapper>* pPlugin = nullptr;
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

				const auto& data = pPlugin->getUnderlyingData();
				_createdPlugins.push_back(pPlugin);
				for (auto bWrapper : data)
				{
					_model.add(bWrapper);
				}
			}
		}
	}
}