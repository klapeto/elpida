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

#include "MatrixInverse4Task.hpp"

#include <random>

namespace Elpida
{

	void MatrixInverse4Task::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_inputData = std::move(inputData);

		_input = Math::Matrix<Float, 4, 4>{
				-0.707, 0.0, -0.707, -13.8,
				0.5, 0.94, 7.0, 8.0,
				9.0, -10.0, -1.0, 1.0,
				945.13, 51.0, 12.1, 121.4
		};	// Don't randomize, we might fall into matrix with no determinant
	}

	SharedPtr<AbstractTaskData> MatrixInverse4Task::Finalize()
	{
		return std::move(_inputData);
	}

	Size MatrixInverse4Task::GetProcessedDataSize() const
	{
		return _matrix.GetRows() * _matrix.GetColumns();
	}

	void MatrixInverse4Task::DoRunImpl()
	{
		Exec([this]()
		{
			_matrix = _input.CalculateInverse();
		});
	}

	Size MatrixInverse4Task::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	UniquePtr<Task> MatrixInverse4Task::DoDuplicate() const
	{
		return std::make_unique<MatrixInverse4Task>();
	}

	TaskInfo MatrixInverse4Task::DoGetInfo() const
	{
		return { "Matrix inverse (4x4)",
				 "Calculates the inverse of a 4x4 matrix",
				 "Numbers",
				 "The number process rate",
				 ResultType::Throughput
		};
	}

}