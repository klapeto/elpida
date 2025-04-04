#ifndef ELPIDA_STRTODBENCHMARK_HPP
#define ELPIDA_STRTODBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class StrtodBenchmark : public Benchmark
	{
	public:
		[[nodiscard]]
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;

		StrtodBenchmark() = default;
		~StrtodBenchmark() override = default;
	protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const override;
		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_STRTODBENCHMARK_HPP
