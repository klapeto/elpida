//
// Created by klapeto on 10/8/2024.
//

#ifndef ELPIDA_RSAENCRYPTIONBENCHMARK_HPP
#define ELPIDA_RSAENCRYPTIONBENCHMARK_HPP

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class RSAEncryptionBenchmark: public Benchmark
	{
	public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		RSAEncryptionBenchmark() = default;
		~RSAEncryptionBenchmark() override = default;
	protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //ELPIDA_RSAENCRYPTIONBENCHMARK_HPP
