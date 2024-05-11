//
// Created by klapeto on 11/5/2024.
//

#ifndef ELPIDA_MEMORYREADBANDWIDTH_HPP
#define ELPIDA_MEMORYREADBANDWIDTH_HPP

#include "Controllers/FullBenchmarkInstance.hpp"


namespace Elpida::Application
{

	class MemoryReadBandwidth : public FullBenchmarkInstance
	{
	public:
		std::string GetName() const override;
		FullBenchmarkInstanceResult Run() override;
		explicit MemoryReadBandwidth(const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				BenchmarkExecutionService& executionService);
	};

} // Application
// Elpida

#endif //ELPIDA_MEMORYREADBANDWIDTH_HPP
