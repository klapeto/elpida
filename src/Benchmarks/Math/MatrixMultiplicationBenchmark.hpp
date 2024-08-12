//
// Created by klapeto on 13/8/2024.
//

#ifndef ELPIDA_MATRIXMULTIPLICATIONBENCHMARK_HPP
#define ELPIDA_MATRIXMULTIPLICATIONBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class MatrixMultiplicationBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		MatrixMultiplicationBenchmark() = default;
		~MatrixMultiplicationBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_MATRIXMULTIPLICATIONBENCHMARK_HPP
