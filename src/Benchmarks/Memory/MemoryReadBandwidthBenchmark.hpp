//
// Created by klapeto on 28/2/2023.
//

#ifndef _MEMORYREADBANDWIDTHBENCHMARK_HPP_
#define _MEMORYREADBANDWIDTHBENCHMARK_HPP_

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class MemoryReadBandwidthBenchmark : public Benchmark
	{
	 public:
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;

		MemoryReadBandwidthBenchmark() = default;
		~MemoryReadBandwidthBenchmark() override = default;
	 protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const override;
		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //_MEMORYREADBANDWIDTHBENCHMARK_HPP_
