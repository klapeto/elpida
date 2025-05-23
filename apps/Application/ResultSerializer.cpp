//
// Created by klapeto on 2/5/2024.
//

#include "ResultSerializer.hpp"
#include "json.hpp"

#include <Elpida/Core/Config.hpp>

#include "Models/SystemInfo/CpuInfoModel.hpp"
#include "Models/SystemInfo/MemoryInfoModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/SystemInfo/TopologyNodeModel.hpp"
#include "Models/SystemInfo/OsInfoModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/Full/FullBenchmarkResultModel.hpp"

using namespace nlohmann;
using namespace Elpida;

namespace Elpida::Application
{

	static nlohmann::json SerializeResult(const MemoryBenchmarkResultModel& resultModel)
	{
		json outJson;

		outJson["size"] = resultModel.GetWorkingSetSize();
		outJson["score"] = resultModel.GetScore();

		return outJson;
	}

	static json SerializeCpuInfo(const CpuInfoModel& cpuInfo)
	{
		json cpu;

		cpu["architecture"] = cpuInfo.GetArchitecture();
		cpu["vendor"] = cpuInfo.GetVendorName();
		cpu["modelName"] = cpuInfo.GetModelName();

		return cpu;
	}

	static json SerializeMemoryInfo(const MemoryInfoModel& memoryInfo)
	{
		json memory;

		memory["pageSize"] = memoryInfo.GetPageSize();
		memory["totalSize"] = memoryInfo.GetTotalSize();

		return memory;
	}

	static json SerializeOsInfo(const OsInfoModel& osInfo)
	{
		json os;

		os["category"] = osInfo.GetCategory();
		os["name"] = osInfo.GetName();
		os["version"] = osInfo.GetVersion();

		return os;
	}

	static json SerializeTimingInfo(const TimingModel& timingInfo)
	{
		json jTiming;

		jTiming["iterations"] = timingInfo.GetIterationsPerSecond();
		jTiming["loopOverhead"] = timingInfo.GetLoopOverhead().count();
		jTiming["nowOverhead"] = timingInfo.GetNowOverhead().count();

		return jTiming;
	}

	static json SerializeElpida()
	{
		json jElpida;

		jElpida["version"] = ELPIDA_VERSION;
		jElpida["compilerName"] = ELPIDA_COMPILER_NAME;
		jElpida["compilerVersion"] = ELPIDA_COMPILER_VERSION;

		return jElpida;
	}

	static json SerializeTopologyNode(const TopologyNodeModel& topologyNode)
	{
		json jNode;

		jNode["type"] = static_cast<int>(topologyNode.GetType());
		if (topologyNode.GetOsIndex().has_value())
		{
			jNode["osIndex"] = topologyNode.GetOsIndex().value();
		}

		if (topologyNode.GetSize().has_value())
		{
			jNode["size"] = *topologyNode.GetSize();
		}

		if (topologyNode.GetEfficiency().has_value())
		{
			jNode["efficiency"] = *topologyNode.GetEfficiency();
		}

		if (!topologyNode.GetMemoryChildren().empty())
		{
			json memoryChildren = json::array();

			for (auto& child : topologyNode.GetMemoryChildren())
			{
				memoryChildren.push_back(SerializeTopologyNode(child));
			}

			jNode["memoryChildren"] = std::move(memoryChildren);
		}

		if (!topologyNode.GetChildren().empty())
		{
			json children = json::array();

			for (auto& child : topologyNode.GetChildren())
			{
				children.push_back(SerializeTopologyNode(child));
			}

			jNode["children"] = std::move(children);
		}

		return jNode;
	}

	static nlohmann::json SerializeResult(const BenchmarkResultModel& resultModel)
	{
		json outJson;

		outJson["uuid"] = resultModel.GetUuid();
		outJson["result"] = resultModel.GetResult();

		return outJson;
	}

	static json SerializeFullBenchmarkResult(const FullBenchmarkResultModel& fullBenchmarkResultModel)
	{
		json outJson;

		outJson["totalScore"] = fullBenchmarkResultModel.GetTotalScore();
		outJson["singleThreadScore"] = fullBenchmarkResultModel.GetSingleThreadScore();
		outJson["multiThreadScore"] = fullBenchmarkResultModel.GetMultiThreadScore();

		json resultsJson = json::array();
		for (auto& result : fullBenchmarkResultModel.GetBenchmarkResults())
		{
			resultsJson.push_back(SerializeResult(result));
		}

		outJson["benchmarkResults"] = std::move(resultsJson);
		return outJson;
	}

	static json SerializeAffinity(const TopologyModel& topologyModel)
	{
		json outJson = json::array();

		for (auto& model : topologyModel.GetSelectedLeafNodes())
		{
			outJson.push_back(model.get().GetOsIndex().value());
		}

		return outJson;
	}

	std::string ResultSerializer::Serialize(
			const std::vector<FullBenchmarkResultModel>& fullBenchmarkResultModels) const
	{
		json outJson = _systemInfo;
		json resultsJson = json::array();

		for (auto& result : fullBenchmarkResultModels)
		{
			resultsJson.push_back(SerializeFullBenchmarkResult(result));
		}

		outJson["results"] = std::move(resultsJson);

		return outJson.dump();
	}

	static json SerializeTopology(const TopologyModel& topologyModel)
	{
		json outJson;

		outJson["root"] = SerializeTopologyNode(topologyModel.GetRoot());
		outJson["totalPackages"] = topologyModel.GetTotalPackages();
		outJson["totalNumaNodes"] = topologyModel.GetTotalNumaNodes();
		outJson["totalPhysicalCores"] = topologyModel.GetTotalPhysicalCores();
		outJson["totalLogicalCores"] = topologyModel.GetTotalLogicalCores();

		return outJson;
	}

	ResultSerializer::ResultSerializer(const CpuInfoModel& cpuInfoModel,
			const MemoryInfoModel& memoryInfoModel, const TopologyModel& topologyModel, const OsInfoModel& osInfoModel,
			const TimingModel& timingModel)
			:_topologyModel(topologyModel)
	{
		_systemInfo["cpu"] = SerializeCpuInfo(cpuInfoModel);
		_systemInfo["memory"] = SerializeMemoryInfo(memoryInfoModel);
		_systemInfo["topology"] = SerializeTopology(topologyModel);
		_systemInfo["os"] = SerializeOsInfo(osInfoModel);
		_systemInfo["timing"] = SerializeTimingInfo(timingModel);
		_systemInfo["elpidaVersion"] = SerializeElpida();
	}

	std::string ResultSerializer::Serialize(const ListModel<BenchmarkResultModel>& benchmarkResultModels) const
	{
		json outJson = _systemInfo;

		json resultsJson = json::array();

		for (auto& result : benchmarkResultModels.GetItems())
		{
			resultsJson.push_back(SerializeResult(result.GetValue()));
		}

		outJson["results"] = std::move(resultsJson);

		return outJson.dump();
	}

	std::string ResultSerializer::Serialize(const ListModel<MemoryBenchmarkResultModel>& benchmarkResultModels) const
	{
		json outJson = _systemInfo;

		json resultsJson = json::array();

		for (auto& result : benchmarkResultModels.GetItems())
		{
			resultsJson.push_back(SerializeResult(result.GetValue()));
		}

		outJson["results"] = std::move(resultsJson);

		return outJson.dump();
	}
} // Application
// Elpida