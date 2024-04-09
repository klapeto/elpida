//
// Created by klapeto on 2/3/2023.
//

#ifndef _PNGENCODINGDECODINGBENCHMARK_HPP_
#define _PNGENCODINGDECODINGBENCHMARK_HPP_

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class PngEncodingDecodingBenchmark : public Benchmark
	{
	 public:
		[[nodiscard]]
		BenchmarkInfo GetInfo() const override;

		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		PngEncodingDecodingBenchmark() = default;
		~PngEncodingDecodingBenchmark() override = default;
	 protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		[[nodiscard]]
		double CalculateScore(const std::vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //_PNGENCODINGDECODINGBENCHMARK_HPP_
