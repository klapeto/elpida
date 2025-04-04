//
// Created by klapeto on 11/8/2024.
//

#ifndef ELPIDA_AESDECRYPTTASK_HPP
#define ELPIDA_AESDECRYPTTASK_HPP

#include <openssl/evp.h>
#include "Elpida/Core/MicroTask.hpp"

namespace Elpida
{

	class AESDecryptTask final : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		explicit AESDecryptTask(std::string key);
		AESDecryptTask(const AESDecryptTask&) = delete;
		AESDecryptTask(AESDecryptTask&&) noexcept = delete;
		AESDecryptTask& operator=(AESDecryptTask&&) noexcept = delete;
		AESDecryptTask& operator=(const AESDecryptTask&) = delete;
		~AESDecryptTask() override = default;
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
		void DoRunImpl() override;
		Size GetOperationsPerformedPerRun() override;
	};

} // Elpida

#endif //ELPIDA_AESDECRYPTTASK_HPP
