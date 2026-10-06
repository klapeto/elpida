//
// Created by klapeto on 4/10/26.
//

#ifndef ELPIDA_INPROCESSBENCHMARKEXECUTIONSERVICE_HPP
#define ELPIDA_INPROCESSBENCHMARKEXECUTIONSERVICE_HPP

#include "Core/BenchmarkExecutionService.hpp"
#include <unordered_map>
#include "Elpida/Core/Benchmark.hpp"


struct ElpidaInstance;

namespace Elpida
{
	class InProcessBenchmarkExecutionService: public Application::BenchmarkExecutionService
	{
	public:
		Application::BenchmarkResultModel Execute(const Application::BenchmarkModel& benchmarkModel,
			const std::vector<std::size_t>& affinity, double nowOverheadSeconds, double loopOverheadSeconds, bool numaAware,
			bool pinThreads, Application::ConcurrencyMode concurrencyMode,
			double minimumMicroTaskDuration) override;
		void StopCurrentExecution() override;

		void SetBenchmark(Benchmark* benchmark)
		{
			_benchmark = benchmark;
		}

		void SetElpidaInstance(ElpidaInstance* instance)
		{
			_instance = instance;
		}

		InProcessBenchmarkExecutionService();
	private:
		ElpidaInstance* _instance;
		Benchmark* _benchmark;
	};

}


#endif //ELPIDA_INPROCESSBENCHMARKEXECUTIONSERVICE_HPP
