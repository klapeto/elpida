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
// Created by klapeto on 17/4/2023.
//

#ifndef ELPIDA_BENCHMARKEXECUTIONSERVICE_HPP_
#define ELPIDA_BENCHMARKEXECUTIONSERVICE_HPP_

#include "Elpida/Platform/Process.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

#include <string>
#include <vector>

namespace Elpida::Application
{
	class BenchmarkModel;

	class BenchmarkExecutionService final
	{
	public:
		BenchmarkResultModel Execute(const BenchmarkModel& benchmarkModel,
				const std::vector<std::size_t>& affinity,
				double nowOverheadSeconds,
				double loopOverheadSeconds,
				bool numaAware,
				bool pinThreads,
				ConcurrencyMode concurrencyMode,
				double minimumMicroTaskDuration);

		void StopCurrentExecution();

		BenchmarkExecutionService() = default;
		~BenchmarkExecutionService() = default;
	private:
		Process _currentProcess;
	};

} // Application

#endif //ELPIDA_BENCHMARKEXECUTIONSERVICE_HPP_
