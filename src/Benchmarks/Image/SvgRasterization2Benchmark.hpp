//
// Created by klapeto on 7/9/2024.
//

#ifndef ELPIDA_SVGRASTERIZATION2BENCHMARK_HPP
#define ELPIDA_SVGRASTERIZATION2BENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class SvgRasterization2Benchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		SvgRasterization2Benchmark() = default;
		~SvgRasterization2Benchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};
} // Elpida

#endif //ELPIDA_SVGRASTERIZATION2BENCHMARK_HPP
