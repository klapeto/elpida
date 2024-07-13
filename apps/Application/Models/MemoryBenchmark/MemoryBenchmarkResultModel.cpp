//
// Created by klapeto on 18/6/2024.
//

#include "MemoryBenchmarkResultModel.hpp"

namespace Elpida::Application
{
	MemoryBenchmarkResultModel::MemoryBenchmarkResultModel(size_t workingSetSize, Score score)
			:_workingSetSize(workingSetSize), _score(score)
	{
	}

	size_t MemoryBenchmarkResultModel::GetWorkingSetSize() const
	{
		return _workingSetSize;
	}

	Score MemoryBenchmarkResultModel::GetScore() const
	{
		return _score;
	}
} // Application
// Elpida