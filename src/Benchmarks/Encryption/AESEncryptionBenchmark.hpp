//
// Created by klapeto on 11/8/2024.
//

#ifndef ELPIDA_AESENCRYPTIONBENCHMARK_HPP
#define ELPIDA_AESENCRYPTIONBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class AESEncryptionBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		AESEncryptionBenchmark() = default;
		~AESEncryptionBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_AESENCRYPTIONBENCHMARK_HPP
