/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 17/9/2024.
//

#include "FullBenchmarkInstancesLoader.hpp"

#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include "BenchmarkBaseScores.hpp"

namespace Elpida::Application
{
	std::vector<std::unique_ptr<FullBenchmarkInstance>> FullBenchmarkInstancesLoader::Load(
			const std::vector<BenchmarkGroupModel>& benchmarkGroups,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			const BenchmarkRunConfigurationModel& runConfigurationModel,
			BenchmarkExecutionService& benchmarkExecutionService,
			std::vector<std::string>& missingBenchmarks)
	{

		auto  generators = BenchmarkData::GetData();

		// wierd code to sort it correctly : Aphabetically and single/multi
		std::vector<std::tuple<std::unique_ptr<FullBenchmarkInstance>, BenchmarkData::GeneratorData, const BenchmarkModel*>> firstBatch;
		for (auto& group : benchmarkGroups)
		{
			for (auto& benchmark : group.GetBenchmarks())
			{
				auto& name = benchmark.GetName();

				auto itr = generators.find(name);
				if (itr != generators.end())
				{
					auto& data = itr->second;
					firstBatch.emplace_back(data.generator(
							data.singleThreadUuid,
							data.singleThreadBaseScore,
							data.multiThreadUuid,
							data.multiThreadBaseScore,
							benchmark,
							timingModel,
							topologyModel,
							memoryInfoModel,
							runConfigurationModel,
							benchmarkExecutionService, false), itr->second, &benchmark);
					data.loaded = true;
				}
			}
		}

		std::sort(firstBatch.begin(), firstBatch.end(),
				[](auto& a, auto& b)
				{
					return std::get<0>(a)->GetName() < std::get<0>(b)->GetName();
				});

		std::vector<std::unique_ptr<FullBenchmarkInstance>> benchmarks;

		std::sort(benchmarks.begin(), benchmarks.end(),
				[](std::unique_ptr<FullBenchmarkInstance>& a, std::unique_ptr<FullBenchmarkInstance>& b)
				{
					return a->GetName() < b->GetName();
				});

		for (auto& pair: firstBatch)
		{
			benchmarks.push_back(std::move(std::get<0>(pair)));
		}

		for (auto& tuple: firstBatch)
		{
			auto& data = std::get<1>(tuple);
			auto& benchmark = *std::get<2>(tuple);
			benchmarks.push_back(data.generator(
					data.singleThreadUuid,
					data.singleThreadBaseScore,
					data.multiThreadUuid,
					data.multiThreadBaseScore,
					benchmark,
					timingModel,
					topologyModel,
					memoryInfoModel,
					runConfigurationModel,
					benchmarkExecutionService, true));
		}

		missingBenchmarks.clear();
		for (auto& generator : generators)
		{
			if (!generator.second.loaded)
			{
				missingBenchmarks.push_back(generator.first);
			}
		}

		return benchmarks;
	}
} // Application
// Elpida