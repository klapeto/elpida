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
