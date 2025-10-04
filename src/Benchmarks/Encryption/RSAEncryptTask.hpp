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
		SharedPtr<AbstractTaskData> _input;
		SharedPtr<AbstractTaskData> _output;
		EvpPKeyCtxPtr _context;
		EvpPKeyPtr _key;
	protected:
		TaskInfo DoGetInfo() const override;
		UniquePtr<Task> DoDuplicate() const override;
		void DoRunImpl() override;
		Size GetOperationsPerformedPerRun() override;
	};

} // Elpida

#endif //ELPIDA_RSAENCRYPTTASK_HPP
