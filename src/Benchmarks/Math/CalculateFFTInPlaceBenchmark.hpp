//
// Created by klapeto on 12/8/2024.
//

#ifndef ELPIDA_CALCULATEFFTINPLACEBENCHMARK_HPP
#define ELPIDA_CALCULATEFFTINPLACEBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class CalculateFFTInPlaceBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		CalculateFFTInPlaceBenchmark() = default;
		~CalculateFFTInPlaceBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_CALCULATEFFTINPLACEBENCHMARK_HPP
