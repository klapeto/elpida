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

		const std::size_t patternSize = 8;

		std::complex<double> pattern[patternSize];
		for (auto& i : pattern)
		{
			i = std::complex<double>(distribution(generator), distribution(generator));
		}

		_values = std::valarray<std::complex<double>>(pattern, totalValues / patternSize);
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

	void CalculateFFTInPlaceTask::DoRun(Iterations iterations)
	{
		while (iterations-- > 0)
		{
			FFTCalculator::CalculateFFTInPlace(_values);
		}
	}

	Size CalculateFFTInPlaceTask::GetOperationsPerformedPerRun()
	{
		return 1;
	}

	Duration CalculateFFTInPlaceTask::GetExecutionMinimumDuration()
	{
		return Seconds(5);
	}
} // Elpida