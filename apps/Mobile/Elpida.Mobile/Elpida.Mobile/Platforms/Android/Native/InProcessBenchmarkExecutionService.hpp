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

#ifndef ELPIDA_INPROCESSBENCHMARKEXECUTIONSERVICE_HPP
#define ELPIDA_INPROCESSBENCHMARKEXECUTIONSERVICE_HPP

#include "Core/BenchmarkExecutionService.hpp"
#include <unordered_map>
#include "Elpida/Core/Benchmark.hpp"


struct ElpidaInstance;

namespace Elpida
{
	class InProcessBenchmarkExecutionService: public Application::BenchmarkExecutionService
	{
	public:
		Application::BenchmarkResultModel Execute(const Application::BenchmarkModel& benchmarkModel,
			const std::vector<std::size_t>& affinity, double nowOverheadSeconds, double loopOverheadSeconds, bool numaAware,
			bool pinThreads, Application::ConcurrencyMode concurrencyMode,
			double minimumMicroTaskDuration) override;
		void StopCurrentExecution() override;

		void SetBenchmark(Benchmark* benchmark)
		{
			_benchmark = benchmark;
		}

		void SetElpidaInstance(ElpidaInstance* instance)
		{
			_instance = instance;
		}

		InProcessBenchmarkExecutionService();
	private:
		ElpidaInstance* _instance;
		Benchmark* _benchmark;
	};

}


#endif //ELPIDA_INPROCESSBENCHMARKEXECUTIONSERVICE_HPP
