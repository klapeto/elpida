//
// Created by klapeto on 17/4/20.
//

#ifndef APPS_QT_MODELS_BENCHMARKRUNNERMODEL_HPP
#define APPS_QT_MODELS_BENCHMARKRUNNERMODEL_HPP


#include "Models/Abstractions/Model.hpp"

namespace Elpida
{
	class TaskSpecification;
	class Benchmark;

	class BenchmarkRunnerModel : public Model
	{
	public:

		void setCurrentRunningTaskSpecification(const TaskSpecification* currentRunningTask)
		{
			_currentRunningTaskSpecification = currentRunningTask;
			onDataChanged();
		}
		void setCurrentRunningTaskBatch(const Benchmark* currentRunningTaskBatch)
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
		void setBenchmarkCompletedTasksCount(size_t benchmarkCompletedTasksCount)
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

		const TaskSpecification* getCurrentRunningTaskSpecification() const
		{
			return _currentRunningTaskSpecification;
		}
		const Benchmark* getCurrentRunningBenchmark() const
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

		BenchmarkRunnerModel()
			: _currentRunningTaskSpecification(nullptr), _currentRunningBenchmark(nullptr), _sessionTotalBenchmarksCount(0),
			  _sessionCompletedBenchmarksCount(0), _benchmarkCompletedTasksCount(0), _benchmarkTotalTasksCount(), _running(false)
		{
		}

		~BenchmarkRunnerModel() override = default;
	private:
		const TaskSpecification* _currentRunningTaskSpecification;
		const Benchmark* _currentRunningBenchmark;
		size_t _sessionTotalBenchmarksCount;
		size_t _sessionCompletedBenchmarksCount;
		size_t _benchmarkCompletedTasksCount;
		size_t _benchmarkTotalTasksCount;

		bool _running;
	};
}


#endif //APPS_QT_MODELS_BENCHMARKRUNNERMODEL_HPP
