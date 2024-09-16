//
// Created by klapeto on 12/8/2024.
//

#ifndef ELPIDA_CALCULATEFFTINPLACETASK_HPP
#define ELPIDA_CALCULATEFFTINPLACETASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include <vector>
#include <complex>

namespace Elpida
{

	class CalculateFFTInPlaceTask final : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		explicit CalculateFFTInPlaceTask(std::size_t size);
		CalculateFFTInPlaceTask(const CalculateFFTInPlaceTask&) = delete;
		CalculateFFTInPlaceTask(CalculateFFTInPlaceTask&&) noexcept = delete;
		CalculateFFTInPlaceTask& operator=(CalculateFFTInPlaceTask&&) noexcept = delete;
		CalculateFFTInPlaceTask& operator=(const CalculateFFTInPlaceTask&) = delete;
		~CalculateFFTInPlaceTask() override = default;
	private:
		std::vector<std::complex<double>> _values;
		SharedPtr<AbstractTaskData> _output;
		std::size_t _size;
	protected:
		TaskInfo DoGetInfo() const override;
		UniquePtr<Task> DoDuplicate() const override;
		void DoRun(Iterations iterations) override;
		Size GetOperationsPerformedPerRun() override;
	};
} // Elpida

#endif //ELPIDA_CALCULATEFFTINPLACETASK_HPP
