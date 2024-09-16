#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}

#[[#include]]# "Controllers/FullBenchmarkInstance.hpp"

namespace Elpida::Application 
{
    class ${NAME}MultiThread : public FullBenchmarkInstance
	{
	public:
		std::string GetName() const override;
		std::string GetUuid() const override;
		FullBenchmarkInstanceResult Run() override;
		explicit ${NAME}MultiThread(const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				BenchmarkExecutionService& executionService);
		~${NAME}MultiThread() override = default;
    };
}

#[[#endif]]# //${INCLUDE_GUARD}
