//
// Created by klapeto on 11/5/2024.
//

#ifndef ELPIDA_MEMORYLATENCY_HPP
#define ELPIDA_MEMORYLATENCY_HPP

#include "Controllers/FullBenchmarkInstance.hpp"

namespace Elpida::Application
{

	class MemoryLatency : public FullBenchmarkInstance
	{
	public:
		std::string GetName() const override;
		std::string GetUuid() const override;
		FullBenchmarkInstanceResult Run() override;
		explicit MemoryLatency(const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				BenchmarkExecutionService& executionService);

		~MemoryLatency() override = default;
	};

} // Application
// Elpida

#endif //ELPIDA_MEMORYLATENCY_HPP
