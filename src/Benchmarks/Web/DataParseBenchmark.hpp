//
// Created by klapeto on 7/4/2024.
//

#ifndef ELPIDA_DATAPARSEBENCHMARK_HPP
#define ELPIDA_DATAPARSEBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class DataParseBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		BenchmarkInfo GetInfo() const override;
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;

		DataParseBenchmark() = default;
		~DataParseBenchmark() override = default;
	protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const override;
		double CalculateScore(const Vector<TaskResult>& taskResults) const override;
	};

} // Elpida

#endif //ELPIDA_DATAPARSEBENCHMARK_HPP
