#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}

#[[#include]]# "Controllers/FullBenchmarkInstance.hpp"

namespace Elpida::Application
{
	class ${NAME} : public FullBenchmarkInstance
	{
	public:
		${NAME}(const std::string& uuid,
				Score baseScore,
				bool multiThreaded,
				const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				const BenchmarkRunConfigurationModel& runConfigurationModel,
				BenchmarkExecutionService& executionService);
	
		${NAME}(const ${NAME}&) = delete;
		${NAME}(${NAME}&&) noexcept = default;
		${NAME}& operator=(const ${NAME}&) = delete;
		${NAME}& operator=(${NAME}&&) noexcept = delete;
		~${NAME}() override = default;
	protected:
		void Configure() override;
	};

} // Elpida::Application

#[[#endif]]# //${INCLUDE_GUARD}
