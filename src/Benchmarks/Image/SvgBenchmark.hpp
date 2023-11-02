//
// Created by klapeto on 2/11/2023.
//

#ifndef ELPIDA_SRC_BENCHMARKS_IMAGE_SVGBENCHMARK_HPP
#define ELPIDA_SRC_BENCHMARKS_IMAGE_SVGBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"
namespace Elpida
{

	class SvgBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		BenchmarkInfo GetInfo() const override;

		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		SvgBenchmark() = default;
		~SvgBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(
			const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors,
			const std::vector<TaskConfiguration>& configuration,
			const EnvironmentInfo& environmentInfo) const override;

		[[nodiscard]]
		double CalculateScore(const std::vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //ELPIDA_SRC_BENCHMARKS_IMAGE_SVGBENCHMARK_HPP
