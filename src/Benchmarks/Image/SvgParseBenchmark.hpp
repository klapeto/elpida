//
// Created by klapeto on 7/4/2024.
//

#ifndef ELPIDA_SVGPARSEBENCHMARK_HPP
#define ELPIDA_SVGPARSEBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class SvgParseBenchmark: public Benchmark
	{
	public:

		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		SvgParseBenchmark() = default;
		~SvgParseBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;
		void DoGetBenchmarkInfo(String& name, String& description, size_t& coreTaskIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_SVGPARSEBENCHMARK_HPP
