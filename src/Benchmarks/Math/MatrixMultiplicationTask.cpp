//
// Created by klapeto on 12/8/2024.
//

#include "MatrixMultiplicationTask.hpp"

#include <random>

namespace Elpida
{
	MatrixMultiplicationTask::MatrixMultiplicationTask(std::size_t rows, std::size_t columns)
			:_rows(rows), _columns(columns)
	{

	}

	void MatrixMultiplicationTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_output = std::move(inputData);

		_a = Matrix(_rows, _columns);
		_b = Matrix(_columns, _rows);

		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());

		std::normal_distribution distribution(0.0, 100.0);

		for (std::size_t i = 0; i < _rows; ++i)
		{
			for (std::size_t j = 0; j < _columns; ++j)
			{
				_a.Get(i, j) = distribution(generator);
				_b.Get(i, j) = distribution(generator);
			}
		}
	}

	SharedPtr<AbstractTaskData> MatrixMultiplicationTask::Finalize()
	{
		return std::move(_output);
	}

	Size MatrixMultiplicationTask::GetProcessedDataSize() const
	{
		return _rows * _columns * 2;
	}

	TaskInfo MatrixMultiplicationTask::DoGetInfo() const
	{
		return {
				"Matrix multiplication",
				"Multiplies 2 matrices",
				"numbers",
				"The amount of numbers processed per second",
				ResultType::Throughput };
	}

	UniquePtr<Task> MatrixMultiplicationTask::DoDuplicate() const
	{
		return std::make_unique<MatrixMultiplicationTask>(_rows, _columns);
	}

	void MatrixMultiplicationTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			_matrix = _a.Multiply(_b);
		}
	}

	Size MatrixMultiplicationTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration MatrixMultiplicationTask::GetExecutionMinimumDuration()
	{
		return Seconds(5);
	}
} // Elpida