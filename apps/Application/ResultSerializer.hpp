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
// Created by klapeto on 2/5/2024.
//

#ifndef ELPIDA_RESULTSERIALIZER_HPP
#define ELPIDA_RESULTSERIALIZER_HPP

#include <string>
#include <vector>
#include "Models/Benchmark/BenchmarkResultModel.hpp"
#include "Models/Full/FullBenchmarkResultModel.hpp"
#include "Models/MemoryBenchmark/MemoryBenchmarkResultModel.hpp"
#include "Models/Abstractions/ListModel/ListModel.hpp"

#include "json.hpp"

namespace Elpida::Application
{
	class CpuInfoModel;
	class MemoryInfoModel;
	class OsInfoModel;
	class TimingModel;
	class TopologyModel;
	class FullBenchmarkResultModel;

	class ResultSerializer
	{
	public:
		[[nodiscard]]
		std::string Serialize(const std::vector<FullBenchmarkResultModel>& fullBenchmarkResultModels) const;

		[[nodiscard]]
		std::string Serialize(const ListModel<BenchmarkResultModel>& benchmarkResultModels) const;

		[[nodiscard]]
		std::string Serialize(const ListModel<MemoryBenchmarkResultModel>& benchmarkResultModels) const;

		explicit ResultSerializer(const CpuInfoModel& cpuInfoModel,
				const MemoryInfoModel& memoryInfoModel,
				const TopologyModel& topologyModel,
				const OsInfoModel& osInfoModel,
				const TimingModel& timingModel);
	private:
		const TopologyModel& _topologyModel;
		nlohmann::json _systemInfo;
	};

} // Elpida

#endif //ELPIDA_RESULTSERIALIZER_HPP
