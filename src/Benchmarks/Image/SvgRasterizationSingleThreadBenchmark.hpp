//
// Created by klapeto on 8/4/2024.
//

#ifndef ELPIDA_SVGRASTERIZATIONSINGLETHREADBENCHMARK_HPP
#define ELPIDA_SVGRASTERIZATIONSINGLETHREADBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class SvgRasterizationSingleThreadBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		BenchmarkInfo GetInfo() const override;

		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		SvgRasterizationSingleThreadBenchmark() = default;
		~SvgRasterizationSingleThreadBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		[[nodiscard]]
		double CalculateScore(const std::vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //ELPIDA_SVGRASTERIZATIONSINGLETHREADBENCHMARK_HPP
