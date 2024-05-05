//
// Created by klapeto on 2/5/2024.
//

#include "ResultSerializer.hpp"
#include "json.hpp"

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
		jTiming["vCallOverhead"] = timingInfo.GetVirtualCallOverhead().count();

		return jTiming;
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

		outJson["name"] = resultModel.GetBenchmark().GetName();
		outJson["score"] = resultModel.GetScore();

		json taskResultsJson = json::array();

		for (auto& taskResult : resultModel.GetTaskResults())
		{
			json resultJson = json();

			resultJson["duration"] = taskResult.GetDuration().count();
			resultJson["inputSize"] = taskResult.GetInputSize();

			taskResultsJson.push_back(std::move(resultJson));
		}

		outJson["taskResults"] = std::move(taskResultsJson);

		return outJson;
	}

	static json SerializeFullBenchmarkResult(const FullBenchmarkResultModel& fullBenchmarkResultModel)
	{
		json outJson;

		outJson["totalScore"] = fullBenchmarkResultModel.GetTotalScore();
		outJson["singleCoreScore"] = fullBenchmarkResultModel.GetSingleCoreScore();
		outJson["multiCoreScore"] = fullBenchmarkResultModel.GetMultiCoreScore();
		outJson["memoryScore"] = fullBenchmarkResultModel.GetMemoryScore();

		json resultsJson = json::array();
		for (auto& result : fullBenchmarkResultModel.GetBenchmarkResults())
		{
			resultsJson.push_back(SerializeResult(result));
		}

		outJson["results"] = std::move(resultsJson);
		return outJson;
	}


	std::string ResultSerializer::Serialize(const ListModel<FullBenchmarkResultModel>& fullBenchmarkResultModels) const
	{
		json outJson;

		outJson["cpu"] = SerializeCpuInfo(_cpuInfoModel);
		outJson["memory"] = SerializeMemoryInfo(_memoryInfoModel);
		outJson["topology"] = SerializeTopologyNode(_topologyModel.GetRoot());
		outJson["os"] = SerializeOsInfo(_osInfoModel);
		outJson["timing"] = SerializeTimingInfo(_timingModel);

		json resultsJson = json::array();
		for (auto& result : fullBenchmarkResultModels.GetItems())
		{
			resultsJson.push_back(SerializeFullBenchmarkResult(result.GetValue()));
		}

		outJson["results"] = std::move(resultsJson);


		return outJson.dump();
	}

	ResultSerializer::ResultSerializer(const CpuInfoModel& cpuInfoModel,
			const MemoryInfoModel& memoryInfoModel, const TopologyModel& topologyModel, const OsInfoModel& osInfoModel,
			const TimingModel& timingModel)
			:_cpuInfoModel(cpuInfoModel), _memoryInfoModel(memoryInfoModel), _topologyModel(topologyModel),
			 _osInfoModel(osInfoModel), _timingModel(timingModel)
	{
	}

	std::string ResultSerializer::Serialize(const ListModel<BenchmarkResultModel>& benchmarkResultModels) const
	{
		json outJson;

		outJson["cpu"] = SerializeCpuInfo(_cpuInfoModel);
		outJson["memory"] = SerializeMemoryInfo(_memoryInfoModel);
		outJson["topology"] = SerializeTopologyNode(_topologyModel.GetRoot());
		outJson["os"] = SerializeOsInfo(_osInfoModel);
		outJson["timing"] = SerializeTimingInfo(_timingModel);

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