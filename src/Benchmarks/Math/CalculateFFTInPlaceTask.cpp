//
// Created by klapeto on 12/8/2024.
//

#include "CalculateFFTInPlaceTask.hpp"
#include "FFTCalculator.hpp"

#include <random>

namespace Elpida
{
	CalculateFFTInPlaceTask::CalculateFFTInPlaceTask(std::size_t size)
			:_size(size)
	{

	}

	void CalculateFFTInPlaceTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		auto totalValues = _size / sizeof(std::complex<double>);
		if (totalValues < 2) totalValues = 2;
		totalValues = std::pow(std::floor(std::sqrt(totalValues)), 2);

		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());

		std::normal_distribution distribution(0.0, 1.0);

		_values.resize(totalValues);
		for (std::size_t i = 0; i < totalValues; ++i)
		{
			_values[i] = std::complex<double>(distribution(generator), distribution(generator));
		}

		_output = std::move(inputData);
	}

	SharedPtr<AbstractTaskData> CalculateFFTInPlaceTask::Finalize()
	{
		return std::move(_output);
	}

	Size CalculateFFTInPlaceTask::GetProcessedDataSize() const
	{
		return _values.size();
	}

	TaskInfo CalculateFFTInPlaceTask::DoGetInfo() const
	{
		return {
				"Calculate FFT in place",
				"Calculates the DTF of a series of complex numbers",
				"numbers",
				"The rate of number processing",
				ResultType::Throughput };
	}

	UniquePtr<Task> CalculateFFTInPlaceTask::DoDuplicate() const
	{
		return std::make_unique<CalculateFFTInPlaceTask>(_size);
	}

	void CalculateFFTInPlaceTask::DoRunImpl()
	{
		Exec([this]()
		{
			FFTCalculator::CalculateFFTInPlace(_values);
		});
	}

	Size CalculateFFTInPlaceTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}
} // Elpida