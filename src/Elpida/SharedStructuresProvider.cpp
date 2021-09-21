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

#include "Elpida/SharedStructuresProvider.hpp"
#include "Elpida/Engine/Calculators/Benchmark/AccumulativeScoreCalculator.hpp"
#include "Elpida/Engine/Calculators/Benchmark/AverageScoreCalculator.hpp"
#include "Elpida/Engine/Calculators/Benchmark/AverageThroughputScoreCalculator.hpp"
#include "Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp"
#include "Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp"
#include "Elpida/CommonTasks/AllocateMemory/AllocateMemorySpecification.hpp"

namespace Elpida
{
	SharedStructuresProvider::SharedStructuresProvider()
	{
		_defaultTasks.emplace(AllocateMemorySpecification::Uuid, std::make_shared<AllocateMemorySpecification>());
		_defaultTasks.emplace(WriteFileSpecification::Uuid, std::make_shared<WriteFileSpecification>());
		_defaultTasks.emplace(ReadFileSpecification::Uuid, std::make_shared<ReadFileSpecification>());
		_accumulativeScoreCalculator = std::make_shared<AccumulativeScoreCalculator>();
		_averageScoreCalculator = std::make_shared<AverageScoreCalculator>();
		_averageThroughputScoreCalculator = std::make_shared<AverageThroughputScoreCalculator>();
	}
}