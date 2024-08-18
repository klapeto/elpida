#ifndef ELPIDA_MALLOCBENCHMARK_HPP
#define ELPIDA_MALLOCBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class MallocBenchmark : public Benchmark
	{
	public:
		[[nodiscard]]
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;

		MallocBenchmark() = default;
		~MallocBenchmark() override = default;
	protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const override;
		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_MALLOCBENCHMARK_HPP
