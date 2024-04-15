//
// Created by klapeto on 15/4/2024.
//

#include <cstdlib>
#include <iostream>
#include <string>
#include <filesystem>

#include "json.hpp"
#include "Elpida/Platform/BenchmarkGroupModule.hpp"
#include <Elpida/Platform/TopologyLoader.hpp>
#include <Elpida/Platform/CpuInfoLoader.hpp>
#include <Elpida/Platform/MemoryInfoLoader.hpp>
#include <Elpida/Platform/OsInfoLoader.hpp>
#include <Elpida/Core/TimingCalculator.hpp>

using namespace nlohmann;
using namespace Elpida;

static json DumpCpuInfo()
{
	json cpu;

	auto info = CpuInfoLoader::Load();

	cpu["architecture"] = info.GetArchitecture();
	cpu["vendor"] = info.GetVendorName();
	cpu["modelName"] = info.GetModelName();

	return cpu;
}

static json DumpTopologyNodeInfo(const TopologyNode& node)
{
	json jNode;

	jNode["type"] = static_cast<int>(node.GetType());
	if (node.GetOsIndex().has_value())
	{
		jNode["osIndex"] = node.GetOsIndex().value();
	}

	switch (node.GetType())
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
		auto& cache = static_cast<const CpuCacheNode&>(node);
		jNode["size"] = cache.GetSize();
	}
		break;
	case NumaDomain:
	{
		auto& numa = static_cast<const NumaNode&>(node);
		jNode["size"] = numa.GetLocalMemorySize();
	}
		break;
	case ProcessingUnit:
	{
		auto& unit = static_cast<const ProcessingUnitNode&>(node);
		if (unit.GetCpuKind().has_value())
		{
			jNode["efficiency"] = unit.GetCpuKind().value().get().GetEfficiency();
		}
	}
		break;
	default:
		break;
	}

	if (!node.GetMemoryChildren().empty())
	{
		json memoryChildren = json::array();

		for (auto& child: node.GetMemoryChildren())
		{
			memoryChildren.push_back(DumpTopologyNodeInfo(*child));
		}

		jNode["memoryChildren"] = std::move(memoryChildren);
	}

	if (!node.GetChildren().empty())
	{
		json children = json::array();

		for (auto& child: node.GetChildren())
		{
			children.push_back(DumpTopologyNodeInfo(*child));
		}

		jNode["children"] = std::move(children);
	}

	return jNode;
}

static json DumpTopologyInfo()
{
	auto info = TopologyLoader::LoadTopology();

	return DumpTopologyNodeInfo(info.GetRoot());
}

static json DumpMemoryInfo()
{
	json memory;

	auto info = MemoryInfoLoader::Load();

	memory["pageSize"] = info.GetPageSize();
	memory["totalSize"] = info.GetTotalSize();

	return memory;
}

static json DumpOsInfo()
{
	json os;

	auto info = OsInfoLoader::Load();

	os["category"] = info.GetCategory();
	os["name"] = info.GetName();
	os["version"] = info.GetVersion();

	return os;
}

static json DumpTask(const TaskInfo& taskInfo)
{

	json jTask;

	jTask["name"] = taskInfo.GetName();
	jTask["description"] = taskInfo.GetDescription();
	jTask["scoreUnit"] = taskInfo.GetScoreUnit();
	jTask["scoreDescription"] = taskInfo.GetScoreDescription();
	jTask["scoreType"] = taskInfo.GetScoreType();
	jTask["isMeasured"] = taskInfo.IsMeasured();

	return jTask;
}


static json DumpBenchmark(const Benchmark& benchmark)
{
	json jBenchmark;
	auto info = benchmark.GetInfo();

	jBenchmark["name"] = info.GetName();
	jBenchmark["description"] = info.GetDescription();
	jBenchmark["scoreUnit"] = info.GetScoreUnit();
	jBenchmark["scoreDescription"] = info.GetScoreDescription();


	json taskArray = json::array();
	for (auto& task: info.GetTaskInfos())
	{
		taskArray.push_back(DumpTask(task));
	}

	jBenchmark["tasks"] = std::move(taskArray);


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

static json DumpBenchmarkGroup(const BenchmarkGroup& group)
{
	json jBenchmarkGroup;

	jBenchmarkGroup["name"] = group.GetName();

	json benchmarkArray = json::array();
	for (auto& benchmark: group.GetBenchmarks())
	{
		benchmarkArray.push_back(DumpBenchmark(*benchmark));
	}

	jBenchmarkGroup["benchmarks"] = std::move(benchmarkArray);

	return jBenchmarkGroup;
}


static json DumpBenchmarks(const std::string& path)
{
	if (!std::filesystem::exists(path))
	{
		return {};
	}

	json benchmarkGroups = json::array();

	for (auto& entry: std::filesystem::directory_iterator(path))
	{
		if (!entry.is_directory() && entry.is_regular_file())
		{
			try
			{
				BenchmarkGroupModule module(entry.path().string());

				benchmarkGroups.push_back(DumpBenchmarkGroup(module.GetBenchmarkGroup()));
			}
			catch (const std::exception& ex)
			{
				// invalid file
			}
		}
	}

	return benchmarkGroups;
}

json DumpTimingInfo()
{
	auto timing = TimingCalculator::CalculateTiming();

	json jTiming;

	jTiming["iterations"] = timing.GetIterationsPerSecond();
	jTiming["loopOverheadNs"] = std::chrono::duration_cast<NanoSeconds>(timing.GetLoopOverhead()).count();
	jTiming["nowOverheadNs"] = std::chrono::duration_cast<NanoSeconds>(timing.GetNowOverhead()).count();
	jTiming["vCallOverheadNs"] = std::chrono::duration_cast<NanoSeconds>(timing.GetVirtualCallOverhead()).count();


	return jTiming;
}

int main(int argC, char** argV)
{
	std::string benchmarkPath;

	if (argC > 1)
	{
		benchmarkPath = argV[1];
	}
	else
	{
		benchmarkPath = "./Benchmarks";
	}

	try
	{
		json root;
		root["cpu"] = DumpCpuInfo();
		root["memory"] = DumpMemoryInfo();
		root["os"] = DumpOsInfo();
		root["topology"] = DumpTopologyInfo();
		root["timing"] = DumpTimingInfo();
		root["benchmarkGroups"] = DumpBenchmarks(benchmarkPath);

		std::cout << root.dump();
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
