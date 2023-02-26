/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021 Ioannis Panagiotopoulos
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
// Created by klapeto on 2/5/21.
//

#ifndef ELPIDA_SHAREDSTRUCTURESPROVIDER_HPP
#define ELPIDA_SHAREDSTRUCTURESPROVIDER_HPP

#include <memory>
#include <unordered_map>

#include "Task/TaskSpecification.hpp"
#include "Calculators/BenchmarkScoreCalculator.hpp"

namespace Elpida
{
	class SharedStructuresProvider final
	{
	public:
		[[nodiscard]]
        std::shared_ptr<BenchmarkScoreCalculator> getAverageBenchmarkScoreCalculator() const
		{
			return _averageScoreCalculator;
		}

        [[nodiscard]]
		std::shared_ptr<BenchmarkScoreCalculator> getAccumulativeBenchmarkScoreCalculator() const
		{
			return _accumulativeScoreCalculator;
		}

        [[nodiscard]]
		std::shared_ptr<BenchmarkScoreCalculator> getAverageThroughputScoreCalculator() const
		{
			return _averageThroughputScoreCalculator;
		}

		SharedStructuresProvider();

		~SharedStructuresProvider() = default;
	private:
		std::shared_ptr<BenchmarkScoreCalculator> _averageScoreCalculator;
		std::shared_ptr<BenchmarkScoreCalculator> _accumulativeScoreCalculator;
		std::shared_ptr<BenchmarkScoreCalculator> _averageThroughputScoreCalculator;
	};
}
#endif //ELPIDA_SHAREDSTRUCTURESPROVIDER_HPP
