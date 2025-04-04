#ifndef ELPIDA_FLOORBENCHMARK_HPP
#define ELPIDA_FLOORBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class FloorBenchmark : public Benchmark
	{
	public:
		[[nodiscard]]
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;

		FloorBenchmark() = default;
		~FloorBenchmark() override = default;
	protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const override;
		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_FLOORBENCHMARK_HPP
