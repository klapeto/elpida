//
// Created by klapeto on 30/7/2024.
//

#ifndef ELPIDA_JSONPARSEBENCHMARK_HPP
#define ELPIDA_JSONPARSEBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class JsonParseBenchmark: public Benchmark
	{
	public:
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;

		JsonParseBenchmark() = default;
		~JsonParseBenchmark() override = default;
	protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const override;
		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_JSONPARSEBENCHMARK_HPP
