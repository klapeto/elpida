//
// Created by klapeto on 1/5/2024.
//

#include "JsonSerializer.hpp"

#include "Elpida/Core/CpuInfo.hpp"
#include "Elpida/Core/MemoryInfo.hpp"
#include "Elpida/Core/OsInfo.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Elpida/Core/Benchmark.hpp"
#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/TaskInfo.hpp"
#include "Elpida/Core/Topology/TopologyInfo.hpp"
#include "Elpida/Core/Topology/TopologyNode.hpp"
#include "Elpida/Core/BenchmarkResult.hpp"

namespace Elpida
{
	using namespace nlohmann;

	nlohmann::json JsonSerializer::Serialize(const CpuInfo& cpuInfo)
	{
		json cpu;

		cpu["architecture"] = cpuInfo.GetArchitecture();
		cpu["vendor"] = cpuInfo.GetVendorName();
		cpu["modelName"] = cpuInfo.GetModelName();

		return cpu;
	}

	nlohmann::json JsonSerializer::Serialize(const TopologyNode& topologyNode)
	{
		json jNode;

		jNode["type"] = static_cast<int>(topologyNode.GetType());
		if (topologyNode.GetOsIndex().has_value())
		{
			jNode["osIndex"] = topologyNode.GetOsIndex().value();
		}

		switch (topologyNode.GetType())
		{
		case L1ICache:
		case L1DCache:
		case L2ICache:
		case L2DCache:
		case L3ICache:
		case L3DCache:
		case L4Cache:
		case L5Cache:
		{
			auto& cache = static_cast<const CpuCacheNode&>(topologyNode);
			jNode["size"] = cache.GetSize();
		}
			break;
		case NumaDomain:
		{
			auto& numa = static_cast<const NumaNode&>(topologyNode);
			jNode["size"] = numa.GetLocalMemorySize();
		}
			break;
		case ProcessingUnit:
		{
			auto& unit = static_cast<const ProcessingUnitNode&>(topologyNode);
			if (unit.GetCpuKind().has_value())
			{
				jNode["efficiency"] = unit.GetCpuKind().value().get().GetEfficiency();
			}
		}
			break;
		default:
			break;
		}

		if (!topologyNode.GetMemoryChildren().empty())
		{
			json memoryChildren = json::array();

			for (auto& child: topologyNode.GetMemoryChildren())
			{
				memoryChildren.push_back(Serialize(*child));
			}

			jNode["memoryChildren"] = std::move(memoryChildren);
		}

		if (!topologyNode.GetChildren().empty())
		{
			json children = json::array();

			for (auto& child: topologyNode.GetChildren())
			{
				children.push_back(Serialize(*child));
			}

			jNode["children"] = std::move(children);
		}

		return jNode;
	}

	nlohmann::json JsonSerializer::Serialize(const TopologyInfo& topologyInfo)
	{
		json topology;
		topology["root"] = Serialize(topologyInfo.GetRoot());
		return topology;
	}

	nlohmann::json JsonSerializer::Serialize(const MemoryInfo& memoryInfo)
	{
		json memory;

		memory["pageSize"] = memoryInfo.GetPageSize();
		memory["totalSize"] = memoryInfo.GetTotalSize();

		return memory;
	}

	nlohmann::json JsonSerializer::Serialize(const OsInfo& osInfo)
	{
		json os;

		os["category"] = osInfo.GetCategory();
		os["name"] = osInfo.GetName();
		os["version"] = osInfo.GetVersion();

		return os;
	}

	nlohmann::json JsonSerializer::Serialize(const TaskInfo& taskInfo)
	{
		json jTask;

		jTask["name"] = taskInfo.GetName();
		jTask["description"] = taskInfo.GetDescription();
		jTask["resultUnit"] = taskInfo.GetResultUnit();
		jTask["resultDescription"] = taskInfo.GetResultDescription();
		jTask["resultType"] = taskInfo.GetResultType();
		jTask["isMeasured"] = taskInfo.IsMeasured();

		return jTask;
	}

	nlohmann::json JsonSerializer::Serialize(const Benchmark& benchmark)
	{
		json jBenchmark;
		auto info = benchmark.GetInfo();

		//jBenchmark["index"] = index;
		//jBenchmark["uuid"] = info.GetUuid();
		jBenchmark["name"] = info.GetName();
		jBenchmark["description"] = info.GetDescription();
		jBenchmark["resultUnit"] = info.GetResultUnit();
		jBenchmark["resultType"] = info.GetResultType();

//		json taskArray = json::array();
//		for (auto& task: info.GetTaskInfos())
//		{
//			taskArray.push_back(Serialize(task));
//		}
//
//		jBenchmark["tasks"] = std::move(taskArray);


		json jRequiredConfiguration = json::array();

		for (auto& config: benchmark.GetRequiredConfiguration())
		{
			json jConfig;

			jConfig["name"] = config.GetName();
			jConfig["type"] = config.GetType();
			jConfig["defaultValue"] = config.GetValue();

			jRequiredConfiguration.push_back(std::move(jConfig));
		}

		jBenchmark["requiredConfiguration"] = std::move(jRequiredConfiguration);

		return jBenchmark;
	}

	nlohmann::json JsonSerializer::Serialize(const BenchmarkGroup& benchmarkGroup)
	{
		json jBenchmarkGroup;

		jBenchmarkGroup["name"] = benchmarkGroup.GetName();
		//jBenchmarkGroup["filePath"] = modulePath;

		json benchmarkArray = json::array();

		auto& benchmarks = benchmarkGroup.GetBenchmarks();
		for (const auto & benchmark : benchmarks)
		{
			benchmarkArray.push_back(Serialize(*benchmark));
		}

		jBenchmarkGroup["benchmarks"] = std::move(benchmarkArray);

		return jBenchmarkGroup;
	}

	nlohmann::json JsonSerializer::Serialize(const TimingInfo& timingInfo)
	{
		json jTiming;

		jTiming["iterations"] = timingInfo.GetIterationsPerSecond();
		jTiming["loopOverhead"] = timingInfo.GetLoopOverhead().count();
		jTiming["nowOverhead"] = timingInfo.GetNowOverhead().count();

		return jTiming;
	}

	nlohmann::json JsonSerializer::Serialize(const BenchmarkResult& result)
	{

		json rootJ;

		rootJ["result"] = result.GetResult();

		return rootJ;
		//json rootJ;

//		auto arrayJ = json::array();
//
//		auto& taskResults = result.GetTaskResults();
//		for (const auto& taskResult : taskResults)
//		{
//			json resultJ;
//
//			resultJ["duration"] = std::chrono::duration_cast<Seconds>(
//					taskResult.GetDuration()).count();
//			resultJ["dataSize"] = taskResult.GetDataSize();
//
//			arrayJ.push_back(std::move(resultJ));
//		}
		//rootJ["taskResults"] = std::move(arrayJ);

		//return rootJ;
		//return arrayJ;
	}
} // Elpida