//  Copyright (c) 2026  Ioannis Panagiotopoulos
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
// Created by klapeto on 29/6/26.
//

#include "InProcessBenchmarkExecutionService.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "ElpidaInstance.hpp"
#include "Elpida/Core/DefaultAllocatorFactory.hpp"
#include "Models/Benchmark/BenchmarkModel.hpp"

namespace Elpida
{
	class MemchrBenchmark;
}

using namespace Elpida;
using namespace Elpida::Application;

BenchmarkResultModel InProcessBenchmarkExecutionService::Execute(
	const BenchmarkModel& benchmarkModel,
	const std::vector<std::size_t>& affinity,
	double nowOverheadSeconds,
	double loopOverheadSeconds,
	bool numaAware,
	bool pinThreads,
	Application::ConcurrencyMode concurrencyMode,
	double minimumMicroTaskDuration)
{
	if (_benchmark == nullptr)
	{
		throw std::runtime_error("Benchmark is not set");
	}

	std::vector<TaskConfiguration> config;

	for (auto& thisConfig: benchmarkModel.GetConfigurations())
	{
		config.emplace_back(thisConfig.GetName(), static_cast<ConfigurationType>(thisConfig.GetType()), thisConfig.GetValue());
	}

	auto context = BenchmarkRunContext(
		_instance->environmentInfo.GetTopologyInfo().GetAllProcessingUnits(),
		config,
		UniquePtr<AllocatorFactory>(new DefaultAllocatorFactory()),
		_instance->environmentInfo,
		static_cast<ConcurrencyMode>(concurrencyMode),
		false,
		Seconds(minimumMicroTaskDuration));

	auto result = _benchmark->Run(context);

	std::unordered_map<std::string, std::string> outputConfig;

	for (auto& thisConfig: benchmarkModel.GetConfigurations())
	{
		outputConfig[thisConfig.GetName()] = thisConfig.GetValue();
	}

	return BenchmarkResultModel(benchmarkModel, result.GetResult(), std::move(outputConfig));
}

void InProcessBenchmarkExecutionService::StopCurrentExecution()
{
}


InProcessBenchmarkExecutionService::InProcessBenchmarkExecutionService()
	: _instance(nullptr), _benchmark(nullptr)
{

}
