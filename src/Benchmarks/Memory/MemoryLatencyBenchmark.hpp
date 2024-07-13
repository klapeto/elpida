//
// Created by klapeto on 27/2/2023.
//

#ifndef _MEMORYLATENCYBENCHMARK_HPP_
#define _MEMORYLATENCYBENCHMARK_HPP_

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class MemoryLatencyBenchmark : public Benchmark
	{
	 public:
		MemoryLatencyBenchmark() = default;
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;
		~MemoryLatencyBenchmark() override = default;
	 protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //_MEMORYLATENCYBENCHMARK_HPP_
