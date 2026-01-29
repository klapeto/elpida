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
