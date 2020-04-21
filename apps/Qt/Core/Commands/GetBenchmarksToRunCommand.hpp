//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_GETBENCHMARKSTORUNCOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_GETBENCHMARKSTORUNCOMMAND_HPP

#include <vector>
#include "Core/Abstractions/TypedCommand.hpp"
#include <Elpida/Engine/Runner/BenchmarkRunRequest.hpp>

namespace Elpida
{
	class GetBenchmarksToRunCommand final : public TypedCommand<GetBenchmarksToRunCommand>
	{
	public:
		void addBenchmark(const Benchmark& benchmark)
		{
			_benchmarks.push_back(&benchmark);
		}

		[[nodiscard]] const std::vector<const Benchmark*>& getBenchmarks() const
		{
			return _benchmarks;
		}

		GetBenchmarksToRunCommand() = default;
		~GetBenchmarksToRunCommand() override = default;
	private:
		std::vector<const Benchmark*> _benchmarks;
	};

}


#endif //APPS_QT_CORE_COMMANDS_GETBENCHMARKSTORUNCOMMAND_HPP
