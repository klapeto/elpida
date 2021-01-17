/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_GETBENCHMARKSTORUNCOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_GETBENCHMARKSTORUNCOMMAND_HPP

#include <vector>
#include "Core/Abstractions/TypedCommand.hpp"

namespace Elpida
{
	class GetBenchmarksToRunCommand final : public TypedCommand<GetBenchmarksToRunCommand>
	{
	public:
		void addBenchmark(const Benchmark& benchmark)
		{
			_benchmarks.emplace_back(benchmark);
		}

		[[nodiscard]] const std::vector<std::reference_wrapper<const Benchmark>>& getBenchmarks() const
		{
			return _benchmarks;
		}

		GetBenchmarksToRunCommand() = default;
		~GetBenchmarksToRunCommand() override = default;
	private:
		std::vector<std::reference_wrapper<const Benchmark>> _benchmarks;
	};

}

#endif //APPS_QT_CORE_COMMANDS_GETBENCHMARKSTORUNCOMMAND_HPP
