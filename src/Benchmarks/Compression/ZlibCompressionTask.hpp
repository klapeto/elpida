//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 30/7/2024.
//

#ifndef ELPIDA_ZLIBCOMPRESSIONTASK_HPP
#define ELPIDA_ZLIBCOMPRESSIONTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Core/MemoryStream.hpp"

#include "zlib.h"

#include <optional>

namespace Elpida
{

	class ZlibCompressionTask : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		ZlibCompressionTask();
		~ZlibCompressionTask() override;
	protected:
		void DoRunImpl() override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		z_stream _zStream;
		SharedPtr<AbstractTaskData> _input;
		std::optional<MemoryStream> _output;
	};

} // Elpida

#endif //ELPIDA_ZLIBCOMPRESSIONTASK_HPP
