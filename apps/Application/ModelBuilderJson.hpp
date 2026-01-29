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
// Created by klapeto on 16/4/2024.
//

#ifndef ELPIDA_MODELBUILDERJSON_HPP
#define ELPIDA_MODELBUILDERJSON_HPP

#include "Models/SystemInfo/OsInfoModel.hpp"
#include "Models/SystemInfo/MemoryInfoModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/CpuInfoModel.hpp"
#include "Models/Benchmark/BenchmarkGroupModel.hpp"

#include <vector>
#include <tuple>

namespace Elpida::Application
{

	class ModelBuilderJson
	{
	public:
		OsInfoModel& GetOsInfoModel();
		MemoryInfoModel& GetMemoryInfoModel();
		TopologyModel& GetTopologyInfoModel();
		TimingModel& GetTimingModel();
		CpuInfoModel& GetCpuInfoModel();

		const std::vector<std::tuple<std::string, std::string>>& GetFailedToLoadBenchmarkGroups() const;
		std::vector<BenchmarkGroupModel>& GetBenchmarkGroups();
		explicit ModelBuilderJson(const std::string& jsonData);
	private:
		OsInfoModel _osInfoModel;
		MemoryInfoModel _memoryInfoModel;
		TopologyModel _topologyInfoModel;
		TimingModel _timingModel;
		CpuInfoModel _cpuInfoModel;
		std::vector<BenchmarkGroupModel> _benchmarkGroups;
		std::vector<std::tuple<std::string, std::string>> _failedToLoadBenchmarkGroups;
	};

} // Elpida

#endif //ELPIDA_MODELBUILDERJSON_HPP
