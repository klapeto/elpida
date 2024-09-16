//
// Created by klapeto on 12/8/2024.
//

#include "DynamicMatrixMultiplicationTask.hpp"

#include <random>

namespace Elpida
{
	DynamicMatrixMultiplicationTask::DynamicMatrixMultiplicationTask(std::size_t rows, std::size_t columns)
			:_rows(rows), _columns(columns)
	{

	}

	void DynamicMatrixMultiplicationTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_output = std::move(inputData);

		_a = DynamicMatrix(_rows, _columns);
		_b = DynamicMatrix(_columns, _rows);

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

	SharedPtr<AbstractTaskData> DynamicMatrixMultiplicationTask::Finalize()
	{
		return std::move(_output);
	}

	Size DynamicMatrixMultiplicationTask::GetProcessedDataSize() const
	{
		return _rows * _columns * 2;
	}

	TaskInfo DynamicMatrixMultiplicationTask::DoGetInfo() const
	{
		return {
				"Dynamic matrix multiplication",
				"Multiplies 2 matrices that their dimensions are not known in advanced",
				"numbers",
				"The amount of numbers processed per second",
				ResultType::Throughput };
	}

	UniquePtr<Task> DynamicMatrixMultiplicationTask::DoDuplicate() const
	{
		return std::make_unique<DynamicMatrixMultiplicationTask>(_rows, _columns);
	}

	void DynamicMatrixMultiplicationTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			_matrix = _a.Multiply(_b);
		}
	}

	Size DynamicMatrixMultiplicationTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}
} // Elpida