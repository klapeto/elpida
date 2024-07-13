//
// Created by klapeto on 7/4/2024.
//

#ifndef ELPIDA_XMLPARSEBENCHMARK_HPP
#define ELPIDA_XMLPARSEBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class XmlParseBenchmark: public Benchmark
	{
	public:
		Vector<TaskConfiguration> GetRequiredConfiguration() const override;

		XmlParseBenchmark() = default;
		~XmlParseBenchmark() override = default;
	protected:
		[[nodiscard]]
		Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const override;
		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_XMLPARSEBENCHMARK_HPP
