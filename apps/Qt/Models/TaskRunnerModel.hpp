//
// Created by klapeto on 17/4/20.
//

#ifndef APPS_QT_MODELS_TASKRUNNERMODEL_HPP
#define APPS_QT_MODELS_TASKRUNNERMODEL_HPP


#include "Models/Abstractions/Model.hpp"

namespace Elpida
{
	class Task;
	class TaskBatch;

	class TaskRunnerModel : public Model
	{
	public:
//		void setCurrentRunningTask(const Task* currentRunningTask)
//		{
//			_currentRunningTask = currentRunningTask;
//			onDataChanged();
//		}
//
//		void setCurrentRunningTaskBatch(const TaskBatch* currentRunningTaskBatch)
//		{
//			_currentRunningTaskBatch = currentRunningTaskBatch;
//			onDataChanged();
//		}
//
//		void setTotalTasksToRun(int totalTasksToRun)
//		{
//			_totalTasksToRun = totalTasksToRun;
//		}
//
//		void setRunning(bool running)
//		{
//			_running = running;
//			onDataChanged();
//		}
//
//		void setTasksThatRun(int tasksThatRun)
//		{
//			_tasksThatRun = tasksThatRun;
//			onDataChanged();
//		}

		const Task* getCurrentRunningTask() const
		{
			return _currentRunningTask;
		}

		const TaskBatch* getCurrentRunningTaskBatch() const
		{
			return _currentRunningTaskBatch;
		}

		int getTotalTasksToRun() const
		{
			return _totalTasksToRun;
		}

		bool isRunning() const
		{
			return _running;
		}

		int getTasksThatRun() const
		{
			return _tasksThatRun;
		}

	private:
		const Task* _currentRunningTask;
		const TaskBatch* _currentRunningTaskBatch;
		int _totalTasksToRun;
		int _tasksThatRun;

	private:
		bool _running;
	};
}


#endif //APPS_QT_MODELS_TASKRUNNERMODEL_HPP
