#[[#include]]# "${NAME}MultiThread.hpp"

#[[#include]]# "Models/BenchmarkRunConfigurationModel.hpp"

#[[#include]]# "Models/Benchmark/BenchmarkModel.hpp"
#[[#include]]# "Models/SystemInfo/TimingModel.hpp"
#[[#include]]# "Models/SystemInfo/TopologyModel.hpp"
#[[#include]]# "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	const Score BaseScore = 1.98 * std::mega::num;

	std::string ${NAME}MultiThread::GetName() const
	{
		return "${NAME} (Multi Thread)";
	}

	std::string ${NAME}MultiThread::GetUuid() const
	{
		return "";
	}

	FullBenchmarkInstanceResult ${NAME}MultiThread::Run()
	{
		_benchmark.GetConfigurations()[0].SetValue("4096");

		auto benchmarkResult = _executionService.Execute(
				_benchmark,
				{},
				_timingModel.GetNowOverhead().count(),
				_timingModel.GetLoopOverhead().count(),
				false,
				false,
				ConcurrencyMode::ShareInput);

		auto thisResult = benchmarkResult.GetResult() / BaseScore;
		Score multiThreadScore = thisResult;

		return FullBenchmarkInstanceResult(std::move(benchmarkResult), 0, multiThreadScore);
	}

	${NAME}MultiThread::${NAME}MultiThread(const BenchmarkModel& benchmark, const TimingModel& timingModel,
			const TopologyModel& topologyModel, const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance(benchmark, timingModel, topologyModel, memoryInfoModel, executionService)
	{

	}
} // Elpida::Application