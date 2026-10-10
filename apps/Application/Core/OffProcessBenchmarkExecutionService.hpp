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

#ifndef ELPIDA_OFFPROCESSBENCHMARKEXECUTIONSERVICE_HPP
#define ELPIDA_OFFPROCESSBENCHMARKEXECUTIONSERVICE_HPP

#include "BenchmarkExecutionService.hpp"
#include "Elpida/Platform/Process.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

#include <vector>

namespace Elpida::Application
{
	class OffProcessBenchmarkExecutionService : public BenchmarkExecutionService
	{
	public:
		BenchmarkResultModel Execute(const BenchmarkModel& benchmarkModel,
		                             const std::vector<std::size_t>& affinity,
		                             double nowOverheadSeconds,
		                             double loopOverheadSeconds,
		                             bool numaAware,
		                             bool pinThreads,
		                             ConcurrencyMode concurrencyMode,
		                             double minimumMicroTaskDuration) override;

		void StopCurrentExecution() override;

		OffProcessBenchmarkExecutionService() = default;
		~OffProcessBenchmarkExecutionService() override = default;

	private:
		Process _currentProcess;
	};
} // Elpida

#endif //ELPIDA_OFFPROCESSBENCHMARKEXECUTIONSERVICE_HPP
