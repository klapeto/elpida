//
// Created by klapeto on 23/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_SELECTEDBENCHMARKCHANGEDEVENT_HPP
#define APPS_QT_CORE_COMMANDS_SELECTEDBENCHMARKCHANGEDEVENT_HPP

#include <Elpida/Engine/Benchmark.hpp>
#include "Core/Abstractions/TypedCommand.hpp"

namespace Elpida
{
	class Benchmark;

	class SelectedBenchmarkChangedEvent : public TypedCommand<SelectedBenchmarkChangedEvent>
	{
	public:
		[[nodiscard]] Benchmark& getCurrentBenchmark() const
		{
			return _currentBenchmark;
		}
		[[nodiscard]] Benchmark* getPreviousBenchmark() const
		{
			return _previousBenchmark;
		}

		SelectedBenchmarkChangedEvent(Benchmark& currentBenchmark, Benchmark* previousBenchmark)
			: _currentBenchmark(currentBenchmark), _previousBenchmark(previousBenchmark)
		{
		}
		~SelectedBenchmarkChangedEvent() override = default;
	private:
		Benchmark& _currentBenchmark;
		Benchmark* _previousBenchmark;
	};
}


#endif //APPS_QT_CORE_COMMANDS_SELECTEDBENCHMARKCHANGEDEVENT_HPP
