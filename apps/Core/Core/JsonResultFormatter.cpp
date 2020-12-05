/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 21/6/20.
//

#include "JsonResultFormatter.hpp"

#include <Elpida/Config.hpp>
#include <Elpida/Utilities/OsUtilities.hpp>
#include <Elpida/SystemInfo/CpuInfo.hpp>
#include <Elpida/SystemInfo/SystemTopology.hpp>
#include <Elpida/SystemInfo/ProcessorNode.hpp>
#include <Elpida/Engine/Result/BenchmarkResult.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <Elpida/SystemInfo/MemoryInfo.hpp>

#include "json.hpp"

namespace Elpida
{
	using namespace nlohmann;

	JsonResultFormatter::JsonResultFormatter(const SystemTopology& systemTopology,
		const CpuInfo& cpuInfo,
		const OsInfo& osInfo,
		const MemoryInfo& memoryInfo)
		: _systemTopology(systemTopology), _cpuInfo(cpuInfo), _osInfo(osInfo), _memoryInfo(memoryInfo)
	{
	}

	static json getElpida()
	{

		json elpida;
		{
			auto version = json::object();

			version["major"] = ELPIDA_VERSION_MAJOR;
			version["minor"] = ELPIDA_VERSION_MINOR;
			version["revision"] = ELPIDA_VERSION_PATCH;
			version["build"] = 0;

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

	static json getCaches(const std::vector<CpuCache>& caches)
	{
		json cachesJ;

		for (auto& cache: caches)
		{
			auto c = json::object();
			c["name"] = cache.getName();
			c["size"] = cache.getSize();
			c["associativity"] = cache.getAssociativity();
			c["lineSize"] = cache.getLineSize();
			c["linesPerTag"] = cache.getLinesPerTag();
			cachesJ.push_back(c);
		}

		return cachesJ;
	}

	static json getCpuExtensions(const std::vector<CpuFeature>& features)
	{
		json featuresJ;
		for (auto& feature: features)
		{
			if (feature.isSupported())
			{
				featuresJ.push_back(feature.getName());
			}
		}
		return featuresJ;
	}

	static json getAdditionalCpuInfo(const CpuInfo& cpuInfo)
	{
		json info;

		for (const auto& pair: cpuInfo.getAdditionalInformation())
		{
			info[pair.first] = pair.second;
		}

		return info;
	}

	static json getCpu(const CpuInfo& cpuInfo)
	{
		json cpu;

		cpu["vendor"] = cpuInfo.getVendorString();
		cpu["brand"] = cpuInfo.getProcessorBrand();
		cpu["additionalInfo"] = getAdditionalCpuInfo(cpuInfo);
		cpu["frequency"] = (unsigned long)cpuInfo.getFrequency();
		cpu["smt"] = cpuInfo.isSmt();

		cpu["caches"] = getCaches(cpuInfo.getCaches());

		cpu["features"] = getCpuExtensions(cpuInfo.getFeatures());

		return cpu;
	}

	static json getNode(const ProcessorNode& node)
	{
		json nodeJ;

		nodeJ["nodeType"] = node.getType();
		nodeJ["name"] = node.getName();
		nodeJ["osIndex"] = node.isOsIndexValid() ? node.getOsIndex() : 0;
		if (node.getValue() > 0)
		{
			nodeJ["value"] = node.getValue();
		}
		else
		{
			nodeJ["value"] = nullptr;
		}

		{
			if (!node.getChildren().empty())
			{
				auto children = json::array();
				for (auto& child :node.getChildren())
				{
					children.push_back(getNode(child));
				}
				nodeJ["children"] = children;
			}
			else
			{
				nodeJ["children"] = nullptr;
			}
		}

		{
			if (!node.getMemoryChildren().empty())
			{
				auto memoryChildren = json::array();
				for (auto& child :node.getMemoryChildren())
				{
					memoryChildren.push_back(getNode(child));
				}
				nodeJ["memoryChildren"] = memoryChildren;
			}
			else
			{
				nodeJ["memoryChildren"] = nullptr;
			}
		}

		return nodeJ;
	}

	static json getTopology(const SystemTopology& topology)
	{
		json topologyJ;

		topologyJ["totalLogicalCores"] = topology.getTotalLogicalCores();
		topologyJ["totalPhysicalCores"] = topology.getTotalPhysicalCores();
		topologyJ["totalDepth"] = topology.getDepth();

		topologyJ["root"] = getNode(*topology.getRoot());

		return topologyJ;
	}

	static json getMemory(const MemoryInfo& memoryInfo)
	{
		json memoryJ;

		memoryJ["totalSize"] = memoryInfo.getMemorySize();
		memoryJ["pageSize"] = memoryInfo.getPageSize();

		return memoryJ;
	}

	static json getSystem(const OsInfo& osInfo,
		const CpuInfo& cpuInfo,
		const SystemTopology& topology,
		const MemoryInfo& memoryInfo)
	{
		json system;
		{
			auto os = json::object();

			os["category"] = osInfo.category;
			os["name"] = osInfo.name;
			os["version"] = osInfo.version;

			system["os"] = os;
		}

		system["cpu"] = getCpu(cpuInfo);
		system["topology"] = getTopology(topology);
		system["memory"] = getMemory(memoryInfo);

		return system;
	}

	static json getTaskResult(const TaskResult& result)
	{
		json resultJ;

		resultJ["name"] = result.getTaskSpecification().getName();
		resultJ["description"] = result.getTaskSpecification().getDescription();
		resultJ["value"] = result.getMetrics().getResultValue();
		resultJ["time"] = result.getMetrics().getDuration().count();
		resultJ["type"] = result.getTaskSpecification().getResultSpecification().getType();
		resultJ["suffix"] = result.getTaskSpecification().getResultSpecification().getUnit();
		resultJ["inputSize"] = result.getMetrics().getInputDataSize();
		resultJ["inputSuffix"] = result.getTaskSpecification().getInputDataSpecification().getUnit();

		return resultJ;
	}

	static json getResult(const BenchmarkResult& result)
	{
		json resultJ;

		resultJ["name"] = result.getBenchmark().getName();
		{
			auto taskResults = json::array();
			for (auto& taskResult: result.getTaskResults())
			{
				taskResults.push_back(getTaskResult(taskResult));
			}
			resultJ["taskResults"] = taskResults;
		}

		return resultJ;
	}

	static json getAffinity(const TaskAffinity& affinity)
	{
		auto affinityJ = json::array();

		for (auto& processor: affinity.getProcessorNodes())
		{
			affinityJ.push_back(processor->getOsIndex());
		}

		return affinityJ;
	}

	std::string JsonResultFormatter::serialize(const BenchmarkResult& result) const
	{
		json root;
		root["elpida"] = getElpida();
		root["system"] = getSystem(_osInfo, _cpuInfo, _systemTopology, _memoryInfo);
		root["affinity"] = getAffinity(result.getAffinity());
		root["result"] = getResult(result);

		return root.dump();
	}

}