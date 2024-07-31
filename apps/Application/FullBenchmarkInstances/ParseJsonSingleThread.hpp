//
// Created by klapeto on 30/7/2024.
//

#ifndef ELPIDA_PARSEJSONSINGLETHREAD_HPP
#define ELPIDA_PARSEJSONSINGLETHREAD_HPP

#include "Controllers/FullBenchmarkInstance.hpp"

namespace Elpida::Application
{

	class ParseJsonSingleThread: public FullBenchmarkInstance
	{
	public:
		std::string GetName() const override;
		std::string GetUuid() const override;
		FullBenchmarkInstanceResult Run() override;
		explicit ParseJsonSingleThread(const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				BenchmarkExecutionService& executionService);
		~ParseJsonSingleThread() override= default;
	};

} // Application
// Elpida

#endif //ELPIDA_PARSEJSONSINGLETHREAD_HPP
