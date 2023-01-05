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

#include "JsonSerializer.hpp"

#include <Elpida/Config.hpp>
#include <Elpida/SystemInfo/OsInfo.hpp>
#include <Elpida/SystemInfo/CpuInfo.hpp>
#include <Elpida/SystemInfo/SystemTopology.hpp>
#include <Elpida/Engine/Result/BenchmarkResult.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <Elpida/SystemInfo/MemoryInfo.hpp>
#include <Elpida/SystemInfo/TimingInfo.hpp>

#include "json.hpp"

namespace Elpida
{
	using namespace nlohmann;

	JsonResultFormatter::JsonResultFormatter(const SystemTopology& systemTopology,
			const CpuInfo& cpuInfo,
			const OsInfo& osInfo,
			const MemoryInfo& memoryInfo,
			const TimingInfo& timingInfo)
			: _systemTopology(systemTopology),
			  _cpuInfo(cpuInfo),
			  _osInfo(osInfo),
			  _memoryInfo(memoryInfo),
			  _timingInfo(timingInfo)
	{
	}

	static json getSystem(const OsInfo& osInfo,
			const CpuInfo& cpuInfo,
			const SystemTopology& topology,
			const MemoryInfo& memoryInfo,
			const TimingInfo& timingInfo)
	{
		json system;

		system["os"] = JsonSerializer::serialize(osInfo);
		system["cpu"] = JsonSerializer::serialize(cpuInfo);
		system["topology"] = JsonSerializer::serialize(topology);
		system["memory"] = JsonSerializer::serialize(memoryInfo);
		system["timing"] = JsonSerializer::serialize(timingInfo);

		return system;
	}

	static json getBenchmarkResults(std::vector<BenchmarkResult>& results)
	{
		auto rootArray = json::array();
		for (const auto& result: results)
		{
			rootArray.push_back(JsonSerializer::serialize(result));
		}

		return rootArray;
	}

	std::string JsonResultFormatter::serialize(std::vector<BenchmarkResult>& results) const
	{
		json root;

		root["elpidaVersion"] = JsonSerializer::serializeElpidaVersion();
		root["system"] = getSystem(_osInfo, _cpuInfo, _systemTopology, _memoryInfo, _timingInfo);

		root["benchmarkResults"] = getBenchmarkResults(results);

		return root.dump();
	}

	std::string JsonResultFormatter::serializeNoMetadata(std::vector<BenchmarkResult>& results) const
	{
		return getBenchmarkResults(results).dump();
	}
}