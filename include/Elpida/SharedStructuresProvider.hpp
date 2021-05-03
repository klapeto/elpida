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
#include "Engine/Task/TaskSpecification.hpp"
#include "Engine/Calculators/BenchmarkScoreCalculator.hpp"
#include <unordered_map>

namespace Elpida
{
	class SharedStructuresProvider final
	{
	public:
		template<typename T>
		std::shared_ptr<TaskSpecification> getSharedTaskSpecification() const
		{
			auto id = std::string(T::Uuid);
			return _defaultTasks.at(id);
		}

		std::shared_ptr<BenchmarkScoreCalculator> getAverageBenchmarkScoreCalculator() const
		{
			return _averageScoreCalculator;
		}

		std::shared_ptr<BenchmarkScoreCalculator> getAccumulativeBenchmarkScoreCalculator() const
		{
			return _accumulativeScoreCalculator;
		}

		std::shared_ptr<BenchmarkScoreCalculator> getAverageThroughputScoreCalculator() const
		{
			return _averageThroughputScoreCalculator;
		}

		SharedStructuresProvider();

		~SharedStructuresProvider() = default;
	private:
		std::unordered_map<std::string, std::shared_ptr<TaskSpecification>> _defaultTasks;
		std::shared_ptr<BenchmarkScoreCalculator> _averageScoreCalculator;
		std::shared_ptr<BenchmarkScoreCalculator> _accumulativeScoreCalculator;
		std::shared_ptr<BenchmarkScoreCalculator> _averageThroughputScoreCalculator;
	};
}
#endif //ELPIDA_SHAREDSTRUCTURESPROVIDER_HPP
