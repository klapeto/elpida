//
// Created by klapeto on 7/4/2024.
//

#ifndef ELPIDA_SVGPARSEBENCHMARK_HPP
#define ELPIDA_SVGPARSEBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class SvgParseBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		BenchmarkInfo GetInfo() const override;

		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		SvgParseBenchmark() = default;
		~SvgParseBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		[[nodiscard]]
		double CalculateScore(const std::vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //ELPIDA_SVGPARSEBENCHMARK_HPP
