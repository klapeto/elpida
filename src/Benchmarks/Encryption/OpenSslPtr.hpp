//
// Created by klapeto on 11/8/2024.
//

#ifndef ELPIDA_OPENSSLPTR_HPP
#define ELPIDA_OPENSSLPTR_HPP

#include <openssl/types.h>
#include <openssl/evp.h>
#include <memory>
#include <openssl/decoder.h>

namespace Elpida
{

	class EvpCipherCtxPtr : public std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>
	{
	public:
		EvpCipherCtxPtr()
				:unique_ptr(nullptr, &::EVP_CIPHER_CTX_free)
		{

		}

		explicit EvpCipherCtxPtr(EVP_CIPHER_CTX* ptr)
				:unique_ptr(ptr, &::EVP_CIPHER_CTX_free)
		{

		}
	};

	class EvpPKeyPtr : public std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>
	{
	public:
		EvpPKeyPtr()
				:unique_ptr(nullptr, &::EVP_PKEY_free)
		{

		}

		explicit EvpPKeyPtr(EVP_PKEY* ptr)
				:unique_ptr(ptr, &::EVP_PKEY_free)
		{

		}
	};

	class EvpPKeyCtxPtr : public std::unique_ptr<EVP_PKEY_CTX, decltype(&::EVP_PKEY_CTX_free)>
	{
	public:
		EvpPKeyCtxPtr()
				:unique_ptr(nullptr, &::EVP_PKEY_CTX_free)
		{

		}

		explicit EvpPKeyCtxPtr(EVP_PKEY_CTX* ptr)
				:unique_ptr(ptr, &::EVP_PKEY_CTX_free)
		{

		}
	};

	class OSslDecoderCtxPtr : public std::unique_ptr<OSSL_DECODER_CTX, decltype(&::OSSL_DECODER_CTX_free)>
	{
	public:
		OSslDecoderCtxPtr()
				:unique_ptr(nullptr, &::OSSL_DECODER_CTX_free)
		{

		}

		explicit OSslDecoderCtxPtr(OSSL_DECODER_CTX* ptr)
				:unique_ptr(ptr, &::OSSL_DECODER_CTX_free)
		{

		}
	};
}

#endif //ELPIDA_OPENSSLPTR_HPP
