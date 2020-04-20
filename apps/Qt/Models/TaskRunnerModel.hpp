//
// Created by klapeto on 17/4/20.
//

#ifndef APPS_QT_MODELS_TASKRUNNERMODEL_HPP
#define APPS_QT_MODELS_TASKRUNNERMODEL_HPP


#include "Models/Abstractions/Model.hpp"

namespace Elpida
{
	class Task;
	class BenchMark;

	class TaskRunnerModel : public Model
	{
	public:

		void setCurrentRunningTask(const Task* currentRunningTask)
		{
			_currentRunningTask = currentRunningTask;
			onDataChanged();
		}
		void setCurrentRunningTaskBatch(const BenchMark* currentRunningTaskBatch)
		{
			_currentRunningBenchmark = currentRunningTaskBatch;
			onDataChanged();
		}
		void setSessionTotalBenchmarksCount(size_t sessionTotalBenchmarksCount)
		{
			_sessionTotalBenchmarksCount = sessionTotalBenchmarksCount;
			onDataChanged();
		}
		void setSessionCompletedBenchmarksCount(size_t sessionCompletedBenchmarksCount )
		{
			_sessionCompletedBenchmarksCount = sessionCompletedBenchmarksCount ;
			onDataChanged();
		}
		void setBatchExecutedTasksCount(size_t benchmarkCompletedTasksCount)
		{
			_benchmarkCompletedTasksCount = benchmarkCompletedTasksCount;
			onDataChanged();
		}
		void setBenchmarkTotalTasksCount(size_t benchmarkTotalTasksCount)
		{
			_benchmarkTotalTasksCount = benchmarkTotalTasksCount;
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
		const BenchMark* getCurrentRunningBenchmark() const
		{
			return _currentRunningBenchmark;
		}
		size_t getSessionTotalBenchmarksCount() const
		{
			return _sessionTotalBenchmarksCount;
		}
		size_t getSessionCompletedBenchmarksCount() const
		{
			return _sessionCompletedBenchmarksCount;
		}
		size_t getBatchExecutedTasksCount() const
		{
			return _benchmarkCompletedTasksCount;
		}
		size_t getBatchTotalTasksCount() const
		{
			return _benchmarkTotalTasksCount;
		}
		bool isRunning() const
		{
			return _running;
		}

		TaskRunnerModel()
			: _currentRunningTask(nullptr), _currentRunningBenchmark(nullptr), _sessionTotalBenchmarksCount(0),
			  _sessionCompletedBenchmarksCount(0), _benchmarkCompletedTasksCount(0), _benchmarkTotalTasksCount(), _running(false)
		{
		}

		~TaskRunnerModel() = default;
	private:
		const Task* _currentRunningTask;
		const BenchMark* _currentRunningBenchmark;
		size_t _sessionTotalBenchmarksCount;
		size_t _sessionCompletedBenchmarksCount;
		size_t _benchmarkCompletedTasksCount;
		size_t _benchmarkTotalTasksCount;

		bool _running;
	};
}


#endif //APPS_QT_MODELS_TASKRUNNERMODEL_HPP
