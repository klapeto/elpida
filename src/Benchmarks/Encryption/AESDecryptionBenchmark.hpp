//
// Created by klapeto on 11/8/2024.
//

#ifndef ELPIDA_AESDECRYPTIONBENCHMARK_HPP
#define ELPIDA_AESDECRYPTIONBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"


namespace Elpida
{

	class AESDecryptionBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		AESDecryptionBenchmark() = default;
		~AESDecryptionBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_AESDECRYPTIONBENCHMARK_HPP
