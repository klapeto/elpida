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

#ifndef ELPIDA_MATRIXINVERSE4TASK_HPP
#define ELPIDA_MATRIXINVERSE4TASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Math/Matrix.hpp"

namespace Elpida
{

	class MatrixInverse4Task : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;

		[[nodiscard]]
		SharedPtr<AbstractTaskData> Finalize() override;

		[[nodiscard]]
		Size GetProcessedDataSize() const override;

		MatrixInverse4Task() = default;
		~MatrixInverse4Task() override = default;
	protected:
		void DoRunImpl() override;

		[[nodiscard]]
		TaskInfo DoGetInfo() const override;

		[[nodiscard]]
		Size GetOperationsPerformedPerRun() override;

		[[nodiscard]]
		UniquePtr<Task> DoDuplicate() const override;
	private:
		Math::Matrix<double, 4, 4> _matrix;
		Math::Matrix<double, 4, 4> _input;
		SharedPtr<AbstractTaskData> _inputData;
	};

}

#endif //ELPIDA_MATRIXINVERSE4TASK_HPP
