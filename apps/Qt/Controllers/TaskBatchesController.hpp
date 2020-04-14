//
// Created by klapeto on 14/4/20.
//

#ifndef APPS_QT_CONTROLLERS_TASKBATCHESCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_TASKBATCHESCONTROLLER_HPP


#include <Models/Abstractions/CollectionModel.hpp>
#include <Elpida/SharedLibraryLoader.hpp>

namespace Elpida {

	class QtTaskBatchWrapper;
	class Logger;

	template<typename T>
	class TaskBatchesContainerPlugin;

	class TaskBatchesController
	{
	public:

		void reload();
		void destroyAll();

		explicit TaskBatchesController(CollectionModel<QtTaskBatchWrapper*>& model, Logger& logger);
		virtual ~TaskBatchesController();
	private:
		std::vector<TaskBatchesContainerPlugin<QtTaskBatchWrapper>*> _createdPlugins;
		SharedLibraryLoader _libraryLoader;
		std::string _taskBatchPath;
		Logger& _logger;
		CollectionModel<QtTaskBatchWrapper*>& _model;

		void reloadLibraries();
	};
}



#endif //APPS_QT_CONTROLLERS_TASKBATCHESCONTROLLER_HPP
