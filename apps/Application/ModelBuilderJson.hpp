//
// Created by klapeto on 16/4/2024.
//

#ifndef ELPIDA_MODELBUILDERJSON_HPP
#define ELPIDA_MODELBUILDERJSON_HPP

#include "Models/SystemInfo/OsInfoModel.hpp"
#include "Models/SystemInfo/MemoryInfoModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/CpuInfoModel.hpp"
#include "Models/Benchmark/BenchmarkGroupModel.hpp"

#include <vector>

namespace Elpida::Application
{

	class ModelBuilderJson
	{
	public:
		OsInfoModel& GetOsInfoModel();
		MemoryInfoModel& GetMemoryInfoModel();
		TopologyModel& GetTopologyInfoModel();
		TimingModel& GetTimingModel();
		CpuInfoModel& GetCpuInfoModel();
		std::vector<BenchmarkGroupModel>& GetBenchmarkGroups();
		explicit ModelBuilderJson(const std::string& jsonData);
	private:
		OsInfoModel _osInfoModel;
		MemoryInfoModel _memoryInfoModel;
		TopologyModel _topologyInfoModel;
		TimingModel _timingModel;
		CpuInfoModel _cpuInfoModel;
		std::vector<BenchmarkGroupModel> _benchmarkGroups;
	};

} // Elpida

#endif //ELPIDA_MODELBUILDERJSON_HPP
