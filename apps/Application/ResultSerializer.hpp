//
// Created by klapeto on 2/5/2024.
//

#ifndef ELPIDA_RESULTSERIALIZER_HPP
#define ELPIDA_RESULTSERIALIZER_HPP

#include <string>
#include <vector>
#include "Models/Benchmark/BenchmarkResultModel.hpp"
#include "Models/Abstractions/ListModel/ListModel.hpp"

namespace Elpida::Application
{
	class CpuInfoModel;
	class MemoryInfoModel;
	class OsInfoModel;
	class TimingModel;
	class TopologyModel;
	class FullBenchmarkResultModel;

	class ResultSerializer
	{
	public:
		[[nodiscard]]
		std::string Serialize(const std::vector<FullBenchmarkResultModel>& fullBenchmarkResultModels) const;

		[[nodiscard]]
		std::string Serialize(const ListModel<BenchmarkResultModel>& benchmarkResultModels) const;

		explicit ResultSerializer(const CpuInfoModel& cpuInfoModel,
				const MemoryInfoModel& memoryInfoModel, const TopologyModel& topologyModel,
				const OsInfoModel& osInfoModel,
				const TimingModel& timingModel);
	private:
		const CpuInfoModel& _cpuInfoModel;
		const MemoryInfoModel& _memoryInfoModel;
		const TopologyModel& _topologyModel;
		const OsInfoModel& _osInfoModel;
		const TimingModel& _timingModel;
	};

} // Elpida

#endif //ELPIDA_RESULTSERIALIZER_HPP
