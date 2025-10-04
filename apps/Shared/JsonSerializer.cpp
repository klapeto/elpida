/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 1/5/2024.
//

#include "JsonSerializer.hpp"

#include "Elpida/Core/CpuInfo.hpp"
#include "Elpida/Core/MemoryInfo.hpp"
#include "Elpida/Core/OsInfo.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Elpida/Core/Benchmark.hpp"
#include "Elpida/Core/TaskInfo.hpp"
#include "Elpida/Core/Topology/TopologyInfo.hpp"
#include "Elpida/Core/Topology/TopologyNode.hpp"

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

		jBenchmark["name"] = info.GetName();
		jBenchmark["description"] = info.GetDescription();
		jBenchmark["resultUnit"] = info.GetResultUnit();
		jBenchmark["resultType"] = info.GetResultType();

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

	nlohmann::json JsonSerializer::Serialize(const TimingInfo& timingInfo)
	{
		json jTiming;

		jTiming["iterations"] = timingInfo.GetIterationsPerSecond();
		jTiming["loopOverhead"] = timingInfo.GetLoopOverhead().count();
		jTiming["nowOverhead"] = timingInfo.GetNowOverhead().count();

		return jTiming;
	}
} // Elpida