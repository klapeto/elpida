/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2022  Ioannis Panagiotopoulos
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
// Created by klapeto on 4/1/2023.
//

#ifndef APPS_QT_CORE_JSONSERIALIZER_HPP
#define APPS_QT_CORE_JSONSERIALIZER_HPP

#include "json.hpp"

namespace Elpida
{
	class OsInfo;
	class CpuInfo;
	class MemoryInfo;
	class TimingInfo;
	class TaskAffinity;
	class SystemTopology;
	class BenchmarkResult;
	class BasicStatistics;

	class JsonSerializer
	{
	public:
		static nlohmann::json serialize(const CpuInfo& cpuInfo);
		static nlohmann::json serialize(const SystemTopology& topology);
		static nlohmann::json serialize(const MemoryInfo& memoryInfo);
		static nlohmann::json serialize(const TimingInfo& timingInfo);
		static nlohmann::json serialize(const OsInfo& osInfo);
		static nlohmann::json serialize(const BasicStatistics& statistics);
		static nlohmann::json serialize(const TaskAffinity& affinity);
		static nlohmann::json serialize(const BenchmarkResult& result);
		static nlohmann::json serializeElpidaVersion();
	};

} // Elpida

#endif //APPS_QT_CORE_JSONSERIALIZER_HPP
