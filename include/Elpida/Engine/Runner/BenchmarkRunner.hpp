//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RUNNER_BENCHMARKRUNNER_HPP
#define INCLUDE_ELPIDA_ENGINE_RUNNER_BENCHMARKRUNNER_HPP

#include "Elpida/Event.hpp"
#include "Elpida/Engine/Result/BenchmarkResult.hpp"
#include "Elpida/Engine/Benchmark.hpp"
#include "Elpida/Engine/Runner/BenchmarkRunRequest.hpp"
#include <vector>

namespace Elpida {

	class Task;
	class BenchmarkEventArgs;
	class TaskEventArgs;

	class BenchmarkRunner
	{
	public:
		Event<const BenchmarkEventArgs&> benchmarkStarted;
		Event<const BenchmarkEventArgs&> benchmarkEnded;
		Event<const TaskEventArgs&> taskStarted;
		Event<const TaskEventArgs&> taskEnded;

		virtual std::vector<BenchmarkResult> runBenchmarks(const std::vector<BenchmarkRunRequest>& benchmarkRequests, const TaskAffinity& taskAffinity);
		void stopBenchmarking();

		BenchmarkRunner();
		virtual ~BenchmarkRunner() = default;
	private:
		bool _mustStop;
		static void destroyTasks(std::vector<Task*>& tasks);
		static TaskMetrics runTask(Task& task);
		void raiseBenchmarkStarted(const Benchmark& benchmark);
		void raiseBenchmarkEnded(const Benchmark& benchmark);
		void raiseTaskStarted(const Task& benchmark);
		void raiseTasksEnded(const Task& benchmark);
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RUNNER_BENCHMARKRUNNER_HPP
