//
// Created by klapeto on 1/5/2024.
//

#ifndef ELPIDA_JSONFORMATER_HPP
#define ELPIDA_JSONFORMATER_HPP

#include "json.hpp"

namespace Elpida
{
	class CpuInfo;
	class TopologyNode;
	class TopologyInfo;
	class MemoryInfo;
	class OsInfo;
	class TaskInfo;
	class Benchmark;
	class BenchmarkGroup;
	class TimingInfo;
	class BenchmarkResult;

	class JsonSerializer
	{
	public:
		static nlohmann::json Serialize(const CpuInfo& cpuInfo);
		static nlohmann::json Serialize(const TopologyNode& topologyNode);
		static nlohmann::json Serialize(const TopologyInfo& topologyInfo);
		static nlohmann::json Serialize(const MemoryInfo& memoryInfo);
		static nlohmann::json Serialize(const OsInfo& osInfo);
		static nlohmann::json Serialize(const TimingInfo& timingInfo);
		static nlohmann::json Serialize(const TaskInfo& taskInfo);
		static nlohmann::json Serialize(const Benchmark& benchmark);

		JsonSerializer() = delete;
	};

} // Elpida

#endif //ELPIDA_JSONFORMATER_HPP
