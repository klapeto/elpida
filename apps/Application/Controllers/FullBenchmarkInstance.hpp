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
// Created by klapeto on 11/5/2024.
//

#ifndef ELPIDA_FULLBENCHMARKINSTANCE_HPP
#define ELPIDA_FULLBENCHMARKINSTANCE_HPP

#include "Core/BenchmarkScore.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

namespace Elpida::Application
{

	class FullBenchmarkInstanceResult
	{
	public:
		BenchmarkResultModel& GetBenchmarkResult();
		Score GetScore() const;
		FullBenchmarkInstanceResult(BenchmarkResultModel&& benchmarkResult, Score score);
	private:
		BenchmarkResultModel _benchmarkResult;
		Score _score;
	};

	class BenchmarkModel;
	class TimingModel;
	class TopologyModel;
	class BenchmarkExecutionService;
	class MemoryInfoModel;
	class BenchmarkRunConfigurationModel;

	class FullBenchmarkInstance
	{
	public:
		[[nodiscard]]
		const std::string& GetName() const;

		[[nodiscard]]
		const std::string& GetUuid() const;

		[[nodiscard]]
		virtual FullBenchmarkInstanceResult Run();

		[[nodiscard]]
		const BenchmarkModel& GetBenchmark() const
		{
			return _benchmark;
		}

		[[nodiscard]]
		bool IsMultiThread() const;

		FullBenchmarkInstance(const FullBenchmarkInstance&) = delete;
		FullBenchmarkInstance(FullBenchmarkInstance&&) noexcept = default;
		FullBenchmarkInstance& operator=(const FullBenchmarkInstance&) = delete;
		FullBenchmarkInstance& operator=(FullBenchmarkInstance&&) noexcept = delete;
		virtual ~FullBenchmarkInstance() = default;
	protected:
		FullBenchmarkInstance(
				std::string name,
				std::string uuid,
				Score baseScore,
				bool multiThreaded,
				const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				const BenchmarkRunConfigurationModel& runConfigurationModel,
				BenchmarkExecutionService& executionService);

		const BenchmarkModel& _benchmark;
		const TimingModel& _timingModel;
		const TopologyModel& _topologyModel;
		const MemoryInfoModel& _memoryInfoModel;
		const BenchmarkRunConfigurationModel& _runConfigurationModel;
		BenchmarkExecutionService& _executionService;
		std::string _name;
		std::string _uuid;
		Score _baseScore;
		bool _multiThread;

		virtual void Configure() = 0;

		[[nodiscard]]
		virtual ConcurrencyMode GetMultiThreadConcurrencyMode() const
		{
			return ConcurrencyMode::ShareInput;
		}
	private:
		[[nodiscard]]
		FullBenchmarkInstanceResult RunSingleThread() const;

		[[nodiscard]]
		FullBenchmarkInstanceResult RunMultiThread() const;
	};

} // Application
// Elpida

#endif //ELPIDA_FULLBENCHMARKINSTANCE_HPP
