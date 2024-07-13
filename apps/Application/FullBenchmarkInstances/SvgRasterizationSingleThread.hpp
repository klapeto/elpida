//
// Created by klapeto on 11/5/2024.
//

#ifndef ELPIDA_SVGRASTERIZATIONSINGLETHREAD_HPP
#define ELPIDA_SVGRASTERIZATIONSINGLETHREAD_HPP

#include "Controllers/FullBenchmarkInstance.hpp"

namespace Elpida::Application
{
	class SvgRasterizationSingleThread : public FullBenchmarkInstance
	{
	public:
		std::string GetName() const override;
		std::string GetUuid() const override;
		FullBenchmarkInstanceResult Run() override;
		explicit SvgRasterizationSingleThread(const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				BenchmarkExecutionService& executionService);
		~SvgRasterizationSingleThread() override = default;
	};

} // Application
// Elpida

#endif //ELPIDA_SVGRASTERIZATIONSINGLETHREAD_HPP
