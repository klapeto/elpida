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

		void setCurrentRunningTask(const Task* currentRunningTask)
		{
			_currentRunningTask = currentRunningTask;
			onDataChanged();
		}
		void setCurrentRunningTaskBatch(const TaskBatch* currentRunningTaskBatch)
		{
			_currentRunningTaskBatch = currentRunningTaskBatch;
			onDataChanged();
		}
		void setSessionTotalTaskBatchesCount(size_t sessionTotalTaskBatchesCount)
		{
			_sessionTotalTaskBatchesCount = sessionTotalTaskBatchesCount;
			onDataChanged();
		}
		void setSessionExecutedTaskBatchesCount(size_t sessionExecutedTaskBatchesCount)
		{
			_sessionExecutedTaskBatchesCount = sessionExecutedTaskBatchesCount;
			onDataChanged();
		}
		void setBatchExecutedTasksCount(size_t batchExecutedTasksCount)
		{
			_batchExecutedTasksCount = batchExecutedTasksCount;
			onDataChanged();
		}
		void setBatchTotalTasksCount(size_t batchTotalTasksCount)
		{
			_batchTotalTasksCount = batchTotalTasksCount;
			onDataChanged();
		}
		void setRunning(bool running)
		{
			_running = running;
			onDataChanged();
		}

		const Task* getCurrentRunningTask() const
		{
			return _currentRunningTask;
		}
		const TaskBatch* getCurrentRunningTaskBatch() const
		{
			return _currentRunningTaskBatch;
		}
		size_t getSessionTotalTaskBatchesCount() const
		{
			return _sessionTotalTaskBatchesCount;
		}
		size_t getSessionExecutedTaskBatchesCount() const
		{
			return _sessionExecutedTaskBatchesCount;
		}
		size_t getBatchExecutedTasksCount() const
		{
			return _batchExecutedTasksCount;
		}
		size_t getBatchTotalTasksCount() const
		{
			return _batchTotalTasksCount;
		}
		bool isRunning() const
		{
			return _running;
		}

		TaskRunnerModel()
			: _currentRunningTask(nullptr), _currentRunningTaskBatch(nullptr), _sessionTotalTaskBatchesCount(0),
			  _sessionExecutedTaskBatchesCount(0), _batchExecutedTasksCount(0), _batchTotalTasksCount(), _running(false)
		{
		}

		~TaskRunnerModel() = default;
	private:
		const Task* _currentRunningTask;
		const TaskBatch* _currentRunningTaskBatch;
		size_t _sessionTotalTaskBatchesCount;
		size_t _sessionExecutedTaskBatchesCount;
		size_t _batchExecutedTasksCount;
		size_t _batchTotalTasksCount;

		bool _running;
	};
}


#endif //APPS_QT_MODELS_TASKRUNNERMODEL_HPP
