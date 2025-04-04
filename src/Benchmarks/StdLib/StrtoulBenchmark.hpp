#ifndef ELPIDA_STRTOULBENCHMARK_HPP
#define ELPIDA_STRTOULBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class StrtoulBenchmark : public Benchmark
	{
	public:
		[[nodiscard]]
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;

		StrtoulBenchmark() = default;
		~StrtoulBenchmark() override = default;
	protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const override;
		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_STRTOULBENCHMARK_HPP
