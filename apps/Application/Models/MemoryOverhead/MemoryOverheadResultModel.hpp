//
// Created by klapeto on 18/6/2024.
//

#ifndef ELPIDA_MEMORYOVERHEADRESULTMODEL_HPP
#define ELPIDA_MEMORYOVERHEADRESULTMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Core/BenchmarkScore.hpp"
#include <cstddef>

namespace Elpida::Application
{

	class MemoryOverheadResultModel : public Model
	{
	public:
		size_t GetWorkingSetSize() const;
		Score GetExpectedScore() const;
		Score GetActualScore() const;
		double GetOverhead() const;
		size_t GetSubSamples() const;
		double GetScale() const;

		MemoryOverheadResultModel(size_t workingSetSize, std::size_t subSamples, double scale, Score expectedScore, Score actualScore, double overhead);
	private:
		std::size_t _workingSetSize;
		std::size_t _subSamples;
		double _scale;
		Score _expectedScore;
		Score _actualScore;
		double _overhead;
	};

} // Application
// Elpida

#endif //ELPIDA_MEMORYOVERHEADRESULTMODEL_HPP
