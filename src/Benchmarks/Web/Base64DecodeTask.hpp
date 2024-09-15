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

#ifndef ELPIDA_BASE64DECODETASK_HPP
#define ELPIDA_BASE64DECODETASK_HPP

#include "Elpida/Core/MicroTask.hpp"

#include <string>

namespace Elpida
{

	class Base64DecodeTask : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		Base64DecodeTask() = default;
		~Base64DecodeTask() override = default;
	protected:
		void DoRun(Iterations iterations) override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		Duration GetExecutionMinimumDuration() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		std::string _outputData;
		SharedPtr<AbstractTaskData> _inputData;
	};

}

#endif //ELPIDA_BASE64DECODETASK_HPP
