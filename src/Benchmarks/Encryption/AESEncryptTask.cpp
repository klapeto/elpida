//
// Created by klapeto on 11/8/2024.
//

#include "AESEncryptTask.hpp"
#include "Utilities.hpp"
#include "OpenSslPtr.hpp"

#include <openssl/evp.h>
#include <openssl/err.h>

namespace Elpida
{
	AESEncryptTask::AESEncryptTask(std::string key)
			:_key(std::move(key)), _initializationVector{ 0xdb, 0x57, 0x0b, 0x26, 0xda, 0x59, 0x54, 0x0d, 0x6e, 0xb7, 0x89, 0xbc, 0xa1, 0x4c, 0x20, 0xa2 }, _finalSize(0)
	{

	}

	void AESEncryptTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_input = std::move(inputData);

		_output = std::make_shared<RawTaskData>(_input->GetAllocator());
		_output->Allocate(_input->GetSize() + BlockSize);
	}

	SharedPtr<AbstractTaskData> AESEncryptTask::Finalize()
	{
		_output->ReAllocate(_finalSize);
		return std::move(_output);
	}

	Size AESEncryptTask::GetProcessedDataSize() const
	{
		return _input->GetSize();
	}

	TaskInfo AESEncryptTask::DoGetInfo() const
	{
		return {
				"AES encryption",
				"Encrypts data using AES",
				"B",
				"The rate of encryption",
				ResultType::Throughput
		};
	}

	UniquePtr<Task> AESEncryptTask::DoDuplicate() const
	{
		return std::make_unique<AESEncryptTask>(_key);
	}

	void AESEncryptTask::DoRun(Iterations iterations)
	{
		const std::size_t inputSize = _input->GetSize();
		const auto input = _input->GetData();

		const auto out = _output->GetData();
		const auto outputCapacity = _output->GetSize();
		while (iterations-- > 0)
		{
			auto context = EvpCipherCtxPtr(EVP_CIPHER_CTX_new());

			if (EVP_EncryptInit_ex(context.get(), EVP_aes_256_cbc(), nullptr,
					reinterpret_cast<unsigned char*>(_key.data()), _initializationVector) != 1)
			{
				Utilities::ThrowOpenSSLError("Failed to initialize encrypt: ");
			}

			int outSize = outputCapacity;
			if (EVP_EncryptUpdate(context.get(), out, &outSize, input, inputSize) != 1)
			{
				Utilities::ThrowOpenSSLError("Failed to encrypt: ");
			}

			int outSize2 = (int)outputCapacity - outSize;

			if (EVP_EncryptFinal_ex(context.get(), out + outSize, &outSize2) != 1)
			{
				Utilities::ThrowOpenSSLError("Failed to encrypt: ");
			}
			_finalSize = outSize + outSize2;
		}
	}

	Size AESEncryptTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}
} // Elpida