//
// Created by klapeto on 8/8/2024.
//

#ifndef ELPIDA_RSAENCRYPTTASK_HPP
#define ELPIDA_RSAENCRYPTTASK_HPP

#include "Elpida/Core/MicroTask.hpp"

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/decoder.h>
#include <openssl/core_names.h>
#include <openssl/types.h>

#include "OpenSslPtr.hpp"

namespace Elpida
{

	class RSAEncryptTask final : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		explicit RSAEncryptTask() = default;
		RSAEncryptTask(const RSAEncryptTask&) = delete;
		RSAEncryptTask(RSAEncryptTask&&) noexcept = delete;
		RSAEncryptTask& operator=(RSAEncryptTask&&) noexcept = delete;
		RSAEncryptTask& operator=(const RSAEncryptTask&) = delete;
		~RSAEncryptTask() override = default;
	private:
		SharedPtr<AbstractTaskData> _output;
		EvpPKeyCtxPtr _context;
		EvpPKeyPtr _key;
	protected:
		TaskInfo DoGetInfo() const override;
		UniquePtr<Task> DoDuplicate() const override;
		void DoRun(Iterations iterations) override;
		Size GetOperationsPerformedPerRun() override;
	};

} // Elpida

#endif //ELPIDA_RSAENCRYPTTASK_HPP
