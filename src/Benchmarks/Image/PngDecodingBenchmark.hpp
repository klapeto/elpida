//
// Created by klapeto on 2/3/2023.
//

#ifndef _PNGDECODINGBENCHMARK_HPP_
#define _PNGDECODINGBENCHMARK_HPP_

#include "Elpida/Benchmark.hpp"
namespace Elpida
{

	class PngDecodingBenchmark : public Benchmark
	{
	 public:
		BenchmarkInfo GetInfo() const override;
	 protected:
		std::vector<std::unique_ptr<Task>> GetTasks() const override;
		double CalculateScore(const std::vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //_PNGDECODINGBENCHMARK_HPP_
