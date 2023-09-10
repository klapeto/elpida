//
// Created by klapeto on 8/9/2023.
//

#include "DataUploader.hpp"

#include <Elpida/Core/Config.hpp>
#include <Elpida/Core/CpuInfo.hpp>

#include "json.hpp"

namespace Elpida::Application
{
	using namespace nlohmann;

	static json getElpida()
	{

		json elpida;
		{
			auto version = json::object();

			version["major"] = ELPIDA_VERSION_MAJOR;
			version["minor"] = ELPIDA_VERSION_MINOR;
			version["revision"] = ELPIDA_VERSION_REVISION;
			version["build"] = ELPIDA_VERSION_BUILD;

			elpida["version"] = version;
		}

		{
			auto compiler = json::object();

			compiler["name"] = ELPIDA_COMPILER_NAME;
			compiler["version"] = ELPIDA_COMPILER_VERSION;

			elpida["compiler"] = compiler;
		}

		return elpida;
	}

//	static json getCaches(const std::vector<CpuCache>& caches)
//	{
//		json cachesJ;
//
//		for (auto& cache: caches)
//		{
//			auto c = json::object();
//			c["name"] = cache.getName();
//			c["size"] = cache.getSize();
//			c["associativity"] = cache.getAssociativity();
//			c["lineSize"] = cache.getLineSize();
//			c["level"] = cache.getLevel();
//			cachesJ.push_back(c);
//		}
//
//		return cachesJ;
//	}

//	static json getCpuExtensions(const std::vector<std::string>& features)
//	{
//		json featuresJ;
//		for (auto& feature: features)
//		{
//			featuresJ.push_back(feature);
//		}
//		return featuresJ;
//	}
//
//	static json getAdditionalCpuInfo(const Elpida::CpuInfo& cpuInfo)
//	{
//		json info;
//
//		for (const auto& pair: cpuInfo.GetAdditionalInfo())
//		{
//			info[pair.first] = pair.second;
//		}
//
//		return info;
//	}
//
//	static json getCpu(const Elpida::CpuInfo& cpuInfo)
//	{
//		json cpu;
//
//		cpu["architecture"] = cpuInfo.GetArchitecture();
//		cpu["vendor"] = cpuInfo.GetVendorName();
//		cpu["modelName"] = cpuInfo.GetModelName();
//		cpu["additionalInfo"] = getAdditionalCpuInfo(cpuInfo);
//		cpu["frequency"] = (unsigned long)cpuInfo.getFrequency();
//		cpu["modelId"] = cpuInfo.getModelId();
//		cpu["smt"] = cpuInfo.isSmt();
//
//		cpu["caches"] = getCaches(cpuInfo.getCaches());
//
//		cpu["features"] = getCpuExtensions(cpuInfo.getFeatures());
//
//		return cpu;
//	}
//
//	static json getNode(const ProcessorNode& node)
//	{
//		json nodeJ;
//
//		nodeJ["nodeType"] = node.getType();
//		nodeJ["name"] = node.getName();
//		nodeJ["osIndex"] = node.isOsIndexValid() ? node.getOsIndex() : 0;
//
//		if (node.getCpuKind().has_value())
//		{
//			nodeJ["efficiency"] = node.getCpuKind()->get().getEfficiency();
//		}
//		else
//		{
//			nodeJ["efficiency"] = nullptr;
//		}
//
//		if (node.getValue() > 0)
//		{
//			nodeJ["value"] = node.getValue();
//		}
//		else
//		{
//			nodeJ["value"] = nullptr;
//		}
//
//		{
//			if (!node.getChildren().empty())
//			{
//				auto children = json::array();
//				for (auto& child :node.getChildren())
//				{
//					children.push_back(getNode(child));
//				}
//				nodeJ["children"] = children;
//			}
//			else
//			{
//				nodeJ["children"] = nullptr;
//			}
//		}
//
//		{
//			if (!node.getMemoryChildren().empty())
//			{
//				auto memoryChildren = json::array();
//				for (auto& child :node.getMemoryChildren())
//				{
//					memoryChildren.push_back(getNode(child));
//				}
//				nodeJ["memoryChildren"] = memoryChildren;
//			}
//			else
//			{
//				nodeJ["memoryChildren"] = nullptr;
//			}
//		}
//
//		return nodeJ;
//	}
//
//	static json getTopology(const SystemTopology& topology)
//	{
//		json topologyJ;
//
//		topologyJ["totalLogicalCores"] = topology.getTotalLogicalCores();
//		topologyJ["totalPhysicalCores"] = topology.getTotalPhysicalCores();
//		topologyJ["totalNumaNodes"] = topology.getTotalNumaNodes();
//		topologyJ["totalPackages"] = topology.getTotalPackages();
//
//		topologyJ["root"] = getNode(topology.getRoot());
//
//		return topologyJ;
//	}
//
//	static json getMemory(const MemoryInfo& memoryInfo)
//	{
//		json memoryJ;
//
//		memoryJ["totalSize"] = memoryInfo.getMemorySize();
//		memoryJ["pageSize"] = memoryInfo.getPageSize();
//
//		return memoryJ;
//	}
//
//	static json getTimingInfo(const TimingInfo& timingInfo)
//	{
//		json timingJ;
//
//		timingJ["notifyOverhead"] = DurationCast<Seconds>(timingInfo.getNotifyOverhead()).count();
//		timingJ["wakeupOverhead"] = DurationCast<Seconds>(timingInfo.getWakeupOverhead()).count();
//		timingJ["sleepOverhead"] = DurationCast<Seconds>(timingInfo.getSleepOverhead()).count();
//		timingJ["nowOverhead"] = DurationCast<Seconds>(timingInfo.getNowOverhead()).count();
//		timingJ["lockOverhead"] = DurationCast<Seconds>(timingInfo.getLockOverhead()).count();
//		timingJ["loopOverhead"] = DurationCast<Seconds>(timingInfo.getLoopOverhead()).count();
//		timingJ["joinOverhead"] = DurationCast<Seconds>(timingInfo.getJoinOverhead()).count();
//		timingJ["targetTime"] = DurationCast<Seconds>(timingInfo.getTargetTime()).count();
//		timingJ["unstableTiming"] = timingInfo.isTargetTimeFallback();
//
//		return timingJ;
//	}
//
//	static json getSystem(const OsInfo& osInfo,
//		const CpuInfo& cpuInfo,
//		const SystemTopology& topology,
//		const MemoryInfo& memoryInfo,
//		const TimingInfo& timingInfo)
//	{
//		json system;
//		{
//			auto os = json::object();
//
//			os["category"] = osInfo.getCategory();
//			os["name"] = osInfo.getName();
//			os["version"] = osInfo.getVersion();
//
//			system["os"] = os;
//		}
//
//		system["cpu"] = getCpu(cpuInfo);
//		system["topology"] = getTopology(topology);
//		system["memory"] = getMemory(memoryInfo);
//		system["timing"] = getTimingInfo(timingInfo);
//
//		return system;
//	}
//
//	static json getBasicStatistics(const BasicStatistics& statistics)
//	{
//		json returnJson;
//
//		returnJson["sampleSize"] = statistics.sampleSize;
//		returnJson["max"] = statistics.max;
//		returnJson["min"] = statistics.min;
//		returnJson["mean"] = statistics.mean;
//		returnJson["standardDeviation"] = statistics.standardDeviation;
//		returnJson["tau"] = statistics.tau;
//		returnJson["marginOfError"] = statistics.marginOfError;
//
//		return returnJson;
//	}
//
//	template<typename T, typename TCallable>
//	static json getCollection(const T& collection, TCallable callable)
//	{
//		json returnJson = json::array();
//
//		for (const auto& value: collection)
//		{
//			returnJson.push_back(callable(value));
//		}
//
//		return returnJson;
//	}
//
//	static json getProcessedTaskResult(const ProcessedTaskResult& result)
//	{
//		json taskJ;
//
//		auto& spec = result.getTaskSpecification();
//
//		taskJ["uuid"] = spec.getUuid();
//		taskJ["value"] = result.getFinalMetrics().getResultValue();
//		taskJ["time"] = DurationCast<Seconds>(result.getFinalMetrics().getDuration()).count();
//		taskJ["inputSize"] = result.getFinalMetrics().getInputDataSize();
//		taskJ["statistics"] = getBasicStatistics(result.getBasicStatistics());
//
//		return taskJ;
//	}
//
//	static json getResult(const BenchmarkResult& result)
//	{
//		return getCollection(result.getTaskResults(),
//			[](const auto& result)
//			{
//			  return getProcessedTaskResult(result);
//			});
//	}
//
//	static json getAffinity(const TaskAffinity& affinity)
//	{
//		auto affinityJ = json::array();
//
//		for (auto& processor: affinity.getProcessorNodes())
//		{
//			affinityJ.push_back(processor->getOsIndex());
//		}
//
//		return affinityJ;
//	}
//
//	static json getBenchmarkResult(const BenchmarkResult& result)
//	{
//
//		json root;
//
//		root["uuid"] = result.getBenchmark().getUuid();
//		root["timeStamp"] = Timer::timestampToString(result.getTimeStamp());
//		root["affinity"] = getAffinity(result.getAffinity());
//		root["score"] = result.getScore().getValue();
//		root["taskResults"] = getResult(result);
//
//		return root;
//	}
//
//	static std::string serialize(std::vector<BenchmarkResultModel>& results)
//	{
//		json root;
//
//		root["elpidaVersion"] = getElpida();
//		root["system"] = getSystem(_osInfo, _cpuInfo, _systemTopology, _memoryInfo, _timingInfo);
//
//		auto rootArray = json::array();
//		for (const auto& result: results)
//		{
//			rootArray
//				.push_back(getBenchmarkResult(result));
//		}
//
//		root["benchmarkResults"] = rootArray;
//
//		return root.dump();
//	}



	void DataUploader::UploadResult(const BenchmarkModel& benchmark, const BenchmarkResultModel& result)
	{

	}

} // Application