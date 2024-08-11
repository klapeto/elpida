//
// Created by klapeto on 11/8/2024.
//

#ifndef ELPIDA_AESENCRYPTTASK_HPP
#define ELPIDA_AESENCRYPTTASK_HPP

#include <openssl/evp.h>
#include "Elpida/Core/MicroTask.hpp"

namespace Elpida
{

	class AESEncryptTask final : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		explicit AESEncryptTask(std::string key);
		AESEncryptTask(const AESEncryptTask&) = delete;
		AESEncryptTask(AESEncryptTask&&) noexcept = delete;
		AESEncryptTask& operator=(AESEncryptTask&&) noexcept = delete;
		AESEncryptTask& operator=(const AESEncryptTask&) = delete;
		~AESEncryptTask() override = default;
	private:
		static constexpr std::size_t BlockSize = 16;

		SharedPtr<AbstractTaskData> _input;
		SharedPtr<AbstractTaskData> _output;
		std::string _key;
		unsigned char _initializationVector[BlockSize];
		std::size_t _finalSize;
	protected:
		TaskInfo DoGetInfo() const override;
		UniquePtr<Task> DoDuplicate() const override;
		void DoRun(Iterations iterations) override;
		Size GetOperationsPerformedPerRun() override;
		Duration GetExecutionMinimumDuration() override;
	};

} // Elpida

#endif //ELPIDA_AESENCRYPTTASK_HPP
