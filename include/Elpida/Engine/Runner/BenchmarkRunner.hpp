/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RUNNER_BENCHMARKRUNNER_HPP
#define INCLUDE_ELPIDA_ENGINE_RUNNER_BENCHMARKRUNNER_HPP

#include "Elpida/Utilities/Event.hpp"
#include "Elpida/Engine/Result/BenchmarkResult.hpp"
#include "Elpida/Engine/Result/TaskResult.hpp"
#include "Elpida/Engine/Runner/BenchmarkRunRequest.hpp"
#include <vector>

namespace Elpida
{

	class Task;
	class BenchmarkEventArgs;
	class TaskEventArgs;
	class TaskAffinity;
	class TaskBuilder;
	class ServiceProvider;

	class BenchmarkRunner
	{
	public:
		virtual std::vector<BenchmarkResult> runBenchmarks(const std::vector<BenchmarkRunRequest>& benchmarkRequests,
			const TaskAffinity& taskAffinity);
		void stopBenchmarking();

		explicit BenchmarkRunner(const ServiceProvider& serviceProvider);
		virtual ~BenchmarkRunner() = default;
	private:
		const ServiceProvider& _serviceProvider;
		bool _mustStop;
		static TaskResult runTask(Task& task);
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RUNNER_BENCHMARKRUNNER_HPP
