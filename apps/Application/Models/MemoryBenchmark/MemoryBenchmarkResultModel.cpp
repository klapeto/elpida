/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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