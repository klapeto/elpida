//
// Created by klapeto on 18/6/2024.
//

#include "MemoryOverheadResultModel.hpp"


namespace Elpida::Application
{
	MemoryOverheadResultModel::MemoryOverheadResultModel(size_t workingSetSize, std::size_t subSamples, double scale, Score expectedScore,
			Score actualScore, double overhead)
			:_workingSetSize(workingSetSize), _subSamples(subSamples), _scale(scale), _expectedScore(expectedScore), _actualScore(actualScore),
			 _overhead(overhead)
	{
	}

	size_t MemoryOverheadResultModel::GetWorkingSetSize() const
	{
		return _workingSetSize;
	}

	Score MemoryOverheadResultModel::GetExpectedScore() const
	{
		return _expectedScore;
	}

	Score MemoryOverheadResultModel::GetActualScore() const
	{
		return _actualScore;
	}

	double MemoryOverheadResultModel::GetOverhead() const
	{
		return _overhead;
	}

	size_t MemoryOverheadResultModel::GetSubSamples() const
	{
		return _subSamples;
	}

	double MemoryOverheadResultModel::GetScale() const
	{
		return _scale;
	}
} // Application
// Elpida