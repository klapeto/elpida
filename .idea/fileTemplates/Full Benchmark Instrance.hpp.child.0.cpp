#[[#include]]# "${NAME}SingleThread.hpp"

#[[#include]]# "Models/BenchmarkRunConfigurationModel.hpp"

#[[#include]]# "Models/Benchmark/BenchmarkModel.hpp"
#[[#include]]# "Models/SystemInfo/TimingModel.hpp"
#[[#include]]# "Models/SystemInfo/TopologyModel.hpp"
#[[#include]]# "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	const Score BaseScore = 1.98 * std::mega::num;

	std::string ${NAME}SingleThread::GetName() const
	{
		return "${NAME} (Single Thread)";
	}

	std::string ${NAME}SingleThread::GetUuid() const
	{
		return "";
	}

	FullBenchmarkInstanceResult ${NAME}SingleThread::Run()
	{
		_benchmark.GetConfigurations()[0].SetValue("4096");

		auto benchmarkResult = _executionService.Execute(
				_benchmark,
				{ _topologyModel.GetFastestProcessor() },
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				false,
				false,
				ConcurrencyMode::None);

		auto thisResult = benchmarkResult.GetResult() / BaseScore;
		Score singleCoreScore = thisResult;

		return FullBenchmarkInstanceResult(std::move(benchmarkResult), singleCoreScore, 0);
	}

	${NAME}SingleThread::${NAME}SingleThread(const BenchmarkModel& benchmark, const TimingModel& timingModel,
			const TopologyModel& topologyModel, const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}
} // Elpida::Application