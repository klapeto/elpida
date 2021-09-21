/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020 Ioannis Panagiotopoulos
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

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP

#include "ProcessedTaskResult.hpp"
#include "Elpida/Engine/Task/TaskAffinity.hpp"
#include "BenchmarkScore.hpp"

#include <vector>
#include <chrono>
#include <string>
#include <Elpida/Timer.hpp>

namespace Elpida
{
	class Benchmark;

	class BenchmarkResult
	{
	public:
		[[nodiscard]]
		const TimeStamp& getTimeStamp() const
		{
			return _timeStamp;
		}

		[[nodiscard]]
		const Benchmark& getBenchmark() const
		{
			return *_benchmark;
		}

		[[nodiscard]]
		const std::vector<ProcessedTaskResult>& getTaskResults() const
		{
			return _taskResults;
		}

		[[nodiscard]]
		const std::string& getId() const
		{
			return _id;
		}

		[[nodiscard]]
		const BenchmarkScore& getScore() const
		{
			return _score;
		}

		[[nodiscard]]
		const TaskAffinity& getAffinity() const
		{
			return _affinity;
		}

		BenchmarkResult(const Benchmark& benchmark,
				std::vector<ProcessedTaskResult>&& taskResults,
				TaskAffinity affinity,
				BenchmarkScore score);

		BenchmarkResult(BenchmarkResult&&) = default;

		BenchmarkResult& operator=(BenchmarkResult&&) = default;

		BenchmarkResult(const BenchmarkResult&) = default;

		BenchmarkResult& operator=(const BenchmarkResult&) = default;

		~BenchmarkResult() = default;

	private:
		std::vector<ProcessedTaskResult> _taskResults;
		TaskAffinity _affinity;
		BenchmarkScore _score;
		std::string _id;
		TimeStamp _timeStamp;
		const Benchmark* _benchmark;

	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP
