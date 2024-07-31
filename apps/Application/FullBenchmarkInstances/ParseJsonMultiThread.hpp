//
// Created by klapeto on 30/7/2024.
//

#ifndef ELPIDA_PARSEJSONMULTITHREAD_HPP
#define ELPIDA_PARSEJSONMULTITHREAD_HPP

#include "Controllers/FullBenchmarkInstance.hpp"

namespace Elpida::Application
{
	class ParseJsonMultiThread : public FullBenchmarkInstance
	{
	public:
		std::string GetName() const override;
		std::string GetUuid() const override;
		FullBenchmarkInstanceResult Run() override;
		explicit ParseJsonMultiThread(const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				BenchmarkExecutionService& executionService);
		~ParseJsonMultiThread() override = default;
	};

} // Application
// Elpida

#endif //ELPIDA_PARSEJSONMULTITHREAD_HPP
