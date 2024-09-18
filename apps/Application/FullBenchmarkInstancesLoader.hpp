//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 17/9/2024.
//

#ifndef ELPIDA_FULLBENCHMARKINSTANCESLOADER_HPP
#define ELPIDA_FULLBENCHMARKINSTANCESLOADER_HPP

#include <vector>
#include "Models/Benchmark/BenchmarkGroupModel.hpp"
#include "Controllers/FullBenchmarkInstance.hpp"


namespace Elpida::Application
{
	class TimingModel;
	class TopologyModel;
	class MemoryInfoModel;
	class BenchmarkExecutionService;

	class FullBenchmarkInstancesLoader
	{
	public:
		static std::vector<std::unique_ptr<FullBenchmarkInstance>> Load(
				const std::vector<BenchmarkGroupModel>& benchmarkGroups,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				BenchmarkExecutionService& benchmarkExecutionService);
	};

} // Application
// Elpida

#endif //ELPIDA_FULLBENCHMARKINSTANCESLOADER_HPP
