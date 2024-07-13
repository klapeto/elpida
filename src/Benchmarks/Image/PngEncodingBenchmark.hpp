//
// Created by klapeto on 2/3/2023.
//

#ifndef _PNGENCODINGDECODINGBENCHMARK_HPP_
#define _PNGENCODINGDECODINGBENCHMARK_HPP_

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class PngEncodingBenchmark : public Benchmark
	{
	 public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		PngEncodingBenchmark() = default;
		~PngEncodingBenchmark() override = default;
	 protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& coreTaskIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //_PNGENCODINGDECODINGBENCHMARK_HPP_
