//
// Created by klapeto on 16/7/2024.
//

#ifndef ELPIDA_SVGRASTERIZATIONFROMFILEBENCHMARK_HPP
#define ELPIDA_SVGRASTERIZATIONFROMFILEBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class SvgRasterizationFromFileBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		SvgRasterizationFromFileBenchmark() = default;
		~SvgRasterizationFromFileBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_SVGRASTERIZATIONFROMFILEBENCHMARK_HPP
