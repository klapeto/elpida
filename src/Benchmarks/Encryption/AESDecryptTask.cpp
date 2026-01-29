/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 11/8/2024.
//

#include "AESDecryptTask.hpp"

#include "Utilities.hpp"
#include "OpenSslPtr.hpp"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>


namespace Elpida
{

	AESDecryptTask::AESDecryptTask(std::string key)
			:_key(std::move(key)), _initializationVector{  0xdb, 0x57, 0x0b, 0x26, 0xda, 0x59, 0x54, 0x0d, 0x6e, 0xb7, 0x89, 0xbc, 0xa1, 0x4c, 0x20, 0xa2 }, _finalSize(0)
	{

	}

	void AESDecryptTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_input = std::move(inputData);

		_output = std::make_shared<RawTaskData>(_input->GetAllocator());
		_output->Allocate(_input->GetSize());
	}

	SharedPtr<AbstractTaskData> AESDecryptTask::Finalize()
	{
		_output->ReAllocate(_finalSize);
		return std::move(_output);
	}

	Size AESDecryptTask::GetProcessedDataSize() const
	{
		return _input->GetSize();
	}

	TaskInfo AESDecryptTask::DoGetInfo() const
	{
		return {
				"AES decryption",
				"Decrypts data using AES",
				"B",
				"The rate of decryption",
				ResultType::Throughput
		};
	}

	UniquePtr<Task> AESDecryptTask::DoDuplicate() const
	{
		return std::make_unique<AESDecryptTask>(_key);
	}

	void AESDecryptTask::DoRunImpl()
	{
		std::size_t inputSize = _input->GetSize();
		auto input = _input->GetData();

		auto out = _output->GetData();
		auto outputCapacity = _output->GetSize();
		Exec([&]()
		{
			auto context = EvpCipherCtxPtr(EVP_CIPHER_CTX_new());

			if (EVP_DecryptInit_ex(context.get(), EVP_aes_256_cbc(), nullptr,
					reinterpret_cast<unsigned char*>(_key.data()), _initializationVector) != 1)
			{
				Utilities::ThrowOpenSSLError("Failed to initialize decrypt: ");
			}

			int outSize = outputCapacity;
			if (EVP_DecryptUpdate(context.get(), out, &outSize, input, inputSize) != 1)
			{
				Utilities::ThrowOpenSSLError("Failed to decrypt: ");
			}

			int outSize2 = (int)outputCapacity - outSize;

			if (EVP_DecryptFinal_ex(context.get(), out + outSize, &outSize2) != 1)
			{
				Utilities::ThrowOpenSSLError("Failed to decrypt: ");
			}
			_finalSize = outSize + outSize2;
		});
	}

	Size AESDecryptTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

} // Elpida