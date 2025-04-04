//
// Created by klapeto on 10/8/2024.
//

#ifndef ELPIDA_RSADECRYPTIONBENCHMARK_HPP
#define ELPIDA_RSADECRYPTIONBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class RSADecryptionBenchmark : public Benchmark
	{
	public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		RSADecryptionBenchmark() = default;
		~RSADecryptionBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_RSADECRYPTIONBENCHMARK_HPP
