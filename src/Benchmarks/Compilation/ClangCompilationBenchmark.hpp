//
// Created by klapeto on 15/12/2024.
//

#ifndef ELPIDA_CLANGCOMPILATIONBENCHMARK_HPP
#define ELPIDA_CLANGCOMPILATIONBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class ClangCompilationBenchmark : public Benchmark
	{
	public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		ClangCompilationBenchmark() = default;
		~ClangCompilationBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_CLANGCOMPILATIONBENCHMARK_HPP
