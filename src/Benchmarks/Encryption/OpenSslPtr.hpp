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
