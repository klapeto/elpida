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
// Created by klapeto on 23/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_SELECTEDBENCHMARKCHANGEDEVENT_HPP
#define APPS_QT_CORE_COMMANDS_SELECTEDBENCHMARKCHANGEDEVENT_HPP

#include "Core/Abstractions/TypedCommand.hpp"
#include <Elpida/Utilities/OptionalReference.hpp>

namespace Elpida
{
	class Benchmark;

	class SelectedBenchmarkChangedEvent final : public TypedCommand<SelectedBenchmarkChangedEvent>
	{
	public:
		[[nodiscard]] OptionalReference<Benchmark> getCurrentBenchmark() const
		{
			return _currentBenchmark;
		}

		[[nodiscard]] OptionalReference<Benchmark> getPreviousBenchmark() const
		{
			return _previousBenchmark;
		}

		SelectedBenchmarkChangedEvent(OptionalReference<Benchmark> currentBenchmark,
			OptionalReference<Benchmark> previousBenchmark)
			: _currentBenchmark(currentBenchmark), _previousBenchmark(previousBenchmark)
		{
		}
		~SelectedBenchmarkChangedEvent() override = default;
	private:
		OptionalReference<Benchmark> _currentBenchmark;
		OptionalReference<Benchmark> _previousBenchmark;
	};
}

#endif //APPS_QT_CORE_COMMANDS_SELECTEDBENCHMARKCHANGEDEVENT_HPP
