//
// Created by klapeto on 9/8/2024.
//

#ifndef ELPIDA_RSADECRYPTTASK_HPP
#define ELPIDA_RSADECRYPTTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "OpenSslPtr.hpp"

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/decoder.h>
#include <openssl/core_names.h>

namespace Elpida
{

	class RSADecryptTask final : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		explicit RSADecryptTask() = default;
		RSADecryptTask(const RSADecryptTask&) = delete;
		RSADecryptTask(RSADecryptTask&&) noexcept = delete;
		RSADecryptTask& operator=(RSADecryptTask&&) noexcept = delete;
		RSADecryptTask& operator=(const RSADecryptTask&) = delete;
		~RSADecryptTask() override = default;
	private:
		SharedPtr<AbstractTaskData> _output;
		SharedPtr<AbstractTaskData> _input;
		EvpPKeyCtxPtr _context;
		EvpPKeyPtr _key;
	protected:
		TaskInfo DoGetInfo() const override;
		UniquePtr<Task> DoDuplicate() const override;
		void DoRunImpl() override;
		Size GetOperationsPerformedPerRun() override;
	};

} // Elpida

#endif //ELPIDA_RSADECRYPTTASK_HPP
