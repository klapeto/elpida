//
// Created by klapeto on 11/5/2024.
//

#ifndef ELPIDA_SVGRASTERIZATIONMULTITHREAD_HPP
#define ELPIDA_SVGRASTERIZATIONMULTITHREAD_HPP

#include "Controllers/FullBenchmarkInstance.hpp"


namespace Elpida::Application
{

	class SvgRasterizationMultiThread: public FullBenchmarkInstance
	{
	public:
		std::string GetName() const override;
		FullBenchmarkInstanceResult Run() override;
		explicit SvgRasterizationMultiThread(const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				BenchmarkExecutionService& executionService);
	};

} // Application
// Elpida

#endif //ELPIDA_SVGRASTERIZATIONMULTITHREAD_HPP
