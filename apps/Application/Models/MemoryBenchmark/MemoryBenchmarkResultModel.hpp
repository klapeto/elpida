//
// Created by klapeto on 18/6/2024.
//

#ifndef ELPIDA_MEMORYBENCHMARKRESULTMODEL_HPP
#define ELPIDA_MEMORYBENCHMARKRESULTMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Core/BenchmarkScore.hpp"
#include <cstddef>

namespace Elpida::Application
{

	class MemoryBenchmarkResultModel : public Model
	{
	public:
		size_t GetWorkingSetSize() const;
		Score GetScore() const;

		MemoryBenchmarkResultModel(size_t workingSetSize, Score score);
	private:
		std::size_t _workingSetSize;
		Score _score;
	};

} // Application
// Elpida

#endif //ELPIDA_MEMORYBENCHMARKRESULTMODEL_HPP
