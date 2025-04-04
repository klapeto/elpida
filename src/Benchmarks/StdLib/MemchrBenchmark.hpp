#ifndef ELPIDA_MEMCHRBENCHMARK_HPP
#define ELPIDA_MEMCHRBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class MemchrBenchmark : public Benchmark
	{
	public:
		[[nodiscard]]
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;

		MemchrBenchmark() = default;
		~MemchrBenchmark() override = default;
	protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const override;
		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_MEMCHRBENCHMARK_HPP
