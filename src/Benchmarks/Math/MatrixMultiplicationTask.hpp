//
// Created by klapeto on 12/8/2024.
//

#ifndef ELPIDA_MATRIXMULTIPLICATIONTASK_HPP
#define ELPIDA_MATRIXMULTIPLICATIONTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Matrix.hpp"

namespace Elpida
{

	class MatrixMultiplicationTask final : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		explicit MatrixMultiplicationTask(std::size_t rows, std::size_t columns);
		MatrixMultiplicationTask(const MatrixMultiplicationTask&) = delete;
		MatrixMultiplicationTask(MatrixMultiplicationTask&&) noexcept = delete;
		MatrixMultiplicationTask& operator=(MatrixMultiplicationTask&&) noexcept = delete;
		MatrixMultiplicationTask& operator=(const MatrixMultiplicationTask&) = delete;
		~MatrixMultiplicationTask() override = default;
	private:
		std::size_t _rows;
		std::size_t _columns;
		Matrix _matrix;
		Matrix _a;
		Matrix _b;
		SharedPtr<AbstractTaskData> _output;
	protected:
		TaskInfo DoGetInfo() const override;
		UniquePtr<Task> DoDuplicate() const override;
		void DoRun(Iterations iterations) override;
		Size GetOperationsPerformedPerRun() override;
		Duration GetExecutionMinimumDuration() override;
	};

} // Elpida

#endif //ELPIDA_MATRIXMULTIPLICATIONTASK_HPP
