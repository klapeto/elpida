//
// Created by klapeto on 27/2/2023.
//

#ifndef _TESTBENCHMARK_HPP_
#define _TESTBENCHMARK_HPP_

#include "Elpida/Benchmark.hpp"

namespace Elpida
{

	class TestBenchmark : public Benchmark
	{
	 public:
		BenchmarkInfo GetInfo() const override;
	 protected:
		std::vector<std::unique_ptr<Task>> GetTasks() const override;
		double CalculateScore(const std::vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //_TESTBENCHMARK_HPP_
