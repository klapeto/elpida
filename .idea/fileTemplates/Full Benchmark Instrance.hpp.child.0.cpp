#[[#include]]# "${NAME}.hpp"
#[[#include]]# "Elpida/Platform/OsUtilities.hpp"
#[[#include]]# "Models/Benchmark/BenchmarkModel.hpp"

namespace Elpida::Application
{
	${NAME}::${NAME}(const std::string& uuid,
			Score baseScore,
			bool multiThreaded,
			const BenchmarkModel& benchmark,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			const BenchmarkRunConfigurationModel& runConfigurationModel,
			BenchmarkExecutionService& executionService)
			:FullBenchmarkInstance("${NAME}", uuid, baseScore, multiThreaded, benchmark, timingModel,
			topologyModel,
			memoryInfoModel, 
			runConfigurationModel, 
			executionService)
	{
	}

	void ${NAME}::Configure()
	{
		//_benchmark.GetConfigurations()[0].SetValue((OsUtilities::GetExecutableDirectory() / "assets" / "lorem-ipsum.enc").string());
		//_benchmark.GetConfigurations()[1].SetValue("ab909b43d4cc43388ed2c98d261b082");
		//_benchmark.GetConfigurations()[2].SetValue("./lorem-ipsum.dec");
	}
} // Elpida::Application