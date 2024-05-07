//
// Created by klapeto on 9/4/2024.
//

#ifndef ELPIDA_SVGRASTERIZATIONBENCHMARK_HPP
#define ELPIDA_SVGRASTERIZATIONBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class SvgRasterizationBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		BenchmarkInfo GetInfo() const override;

		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		SvgRasterizationBenchmark() = default;
		~SvgRasterizationBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		[[nodiscard]]
		double CalculateScore(const std::vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //ELPIDA_SVGRASTERIZATIONBENCHMARK_HPP
