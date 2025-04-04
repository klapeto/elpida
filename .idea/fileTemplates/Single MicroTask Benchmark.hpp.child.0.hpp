#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}

#[[#include]]# "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class ${NAME}Benchmark: public Benchmark
	{
	public:
	    [[nodiscard]]
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;

		${NAME}Benchmark() = default;
		~${NAME}Benchmark() override = default;
	protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const override;
		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#[[#endif]]# //${INCLUDE_GUARD}
