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
