//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Result/BenchmarkResult.hpp"
#include "Elpida/Utilities/Uuid.hpp"

namespace Elpida
{

	BenchmarkResult::BenchmarkResult(const Benchmark& benchmark,
		std::vector<TaskResult>&& taskResults,
		BenchmarkResult::Score score)
		: _benchmark(&benchmark), _taskResults(std::move(taskResults)), _score(score)
	{
		_id = Uuid::create();
	}
}