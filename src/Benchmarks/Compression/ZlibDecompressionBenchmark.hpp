//
// Created by klapeto on 31/7/2024.
//

#ifndef ELPIDA_ZLIBDECOMPRESSIONBENCHMARK_HPP
#define ELPIDA_ZLIBDECOMPRESSIONBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class ZlibDecompressionBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		ZlibDecompressionBenchmark() = default;
		~ZlibDecompressionBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_ZLIBDECOMPRESSIONBENCHMARK_HPP
