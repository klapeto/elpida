//
// Created by klapeto on 22/8/2023.
//

#ifndef ELPIDA_BENCHMARKRESULTMODEL_HPP
#define ELPIDA_BENCHMARKRESULTMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Elpida/Core/Vector.hpp"
#include "TaskResultModel.hpp"

namespace Elpida::Application
{

	class BenchmarkModel;

	class BenchmarkResultModel final : public Model
	{
	public:

		const Vector<TaskResultModel>& GetTaskResults() const
		{
			return _taskResults;
		}

		double GetScore() const
		{
			return _score;
		}

		const BenchmarkModel& GetBenchmark() const
		{
			return *_benchmark;
		}

		BenchmarkResultModel(const BenchmarkModel& benchmark, double score, Vector<TaskResultModel>&& taskResults)
			: _taskResults(std::move(taskResults)), _benchmark(&benchmark), _score(score)
		{

		}

		BenchmarkResultModel(const BenchmarkResultModel&) = delete;
		BenchmarkResultModel(BenchmarkResultModel&&) noexcept = default;
		BenchmarkResultModel& operator=(const BenchmarkResultModel&) = delete;
		BenchmarkResultModel& operator=(BenchmarkResultModel&&) noexcept = default;
		~BenchmarkResultModel() override = default;
	private:
		Vector<TaskResultModel> _taskResults;
		const BenchmarkModel* _benchmark;
		double _score;
	};

} // Application

#endif //ELPIDA_BENCHMARKRESULTMODEL_HPP
