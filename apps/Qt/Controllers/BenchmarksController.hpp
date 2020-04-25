//
// Created by klapeto on 14/4/20.
//

#ifndef APPS_QT_CONTROLLERS_BENCHMARKSCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_BENCHMARKSCONTROLLER_HPP


#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Models/Abstractions/AssociativeModel/AssociativeModel.hpp"
#include <Elpida/Engine/Configuration/BenchmarkConfiguration.hpp>
#include <Elpida/SharedLibraryLoader.hpp>

namespace Elpida
{
	class Logger;
	class Benchmark;

	template<typename T>
	class TaskBatchesContainerPlugin;

	class BenchmarksController
	{
	public:

		void reload();
		void destroyAll();

		explicit BenchmarksController(ListModel<Benchmark*>& model, AssociativeModel<std::string, BenchmarkConfiguration>& configurationsModel, Logger& logger);
		virtual ~BenchmarksController();
	private:
		std::vector<TaskBatchesContainerPlugin<Benchmark>*> _createdPlugins;
		SharedLibraryLoader _libraryLoader;
		std::string _benchmarksPath;
		Logger& _logger;
		ListModel<Benchmark*>& _model;
		AssociativeModel<std::string, BenchmarkConfiguration>& _configurationsModel;

		void reloadLibraries();
	};
}


#endif //APPS_QT_CONTROLLERS_BENCHMARKSCONTROLLER_HPP
