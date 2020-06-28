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

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP

#include "TaskResult.hpp"
#include "Elpida/Engine/Task/TaskAffinity.hpp"

#include <vector>
#include <string>

namespace Elpida
{
	class Benchmark;

	class BenchmarkResult
	{
	public:
		using Score = double;

		[[nodiscard]] const Benchmark& getBenchmark() const
		{
			return *_benchmark;
		}

		[[nodiscard]] const std::vector<TaskResult>& getTaskResults() const
		{
			return _taskResults;
		}

		[[nodiscard]] const std::string& getId() const
		{
			return _id;
		}

		[[nodiscard]] Score getScore() const
		{
			return _score;
		}

		[[nodiscard]] const TaskAffinity& getAffinity() const
		{
			return _affinity;
		}

		BenchmarkResult(const Benchmark& benchmark,
			std::vector<TaskResult>&& taskResults,
			const TaskAffinity& affinity,
			BenchmarkResult::Score score);
		BenchmarkResult(BenchmarkResult&&) = default;
		BenchmarkResult& operator=(BenchmarkResult&&) = default;
		BenchmarkResult(const BenchmarkResult&) = default;
		BenchmarkResult& operator=(const BenchmarkResult&) = default;
		~BenchmarkResult() = default;
	private:
		TaskAffinity _affinity;
		const Benchmark* _benchmark;
		std::vector<TaskResult> _taskResults;
		std::string _id;
		Score _score;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RESULT_BENCHMARKRESULT_HPP
