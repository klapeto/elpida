/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 12/8/2024.
//

#ifndef ELPIDA_DYNAMICMATRIXMULTIPLICATIONTASK_HPP
#define ELPIDA_DYNAMICMATRIXMULTIPLICATIONTASK_HPP

#include "Elpida/Core/MicroTask.hpp"
#include "Elpida/Core/Float.hpp"
#include "Elpida/Math/DynamicMatrix.hpp"

namespace Elpida
{

	using namespace Math;

	class DynamicMatrixMultiplicationTask final : public MicroTask
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		Size GetProcessedDataSize() const override;

		explicit DynamicMatrixMultiplicationTask(std::size_t rows, std::size_t columns);
		DynamicMatrixMultiplicationTask(const DynamicMatrixMultiplicationTask&) = delete;
		DynamicMatrixMultiplicationTask(DynamicMatrixMultiplicationTask&&) noexcept = delete;
		DynamicMatrixMultiplicationTask& operator=(DynamicMatrixMultiplicationTask&&) noexcept = delete;
		DynamicMatrixMultiplicationTask& operator=(const DynamicMatrixMultiplicationTask&) = delete;
		~DynamicMatrixMultiplicationTask() override = default;
	private:
		std::size_t _rows;
		std::size_t _columns;
		DynamicMatrix<Float> _matrix;
		DynamicMatrix<Float> _a;
		DynamicMatrix<Float> _b;
		SharedPtr<AbstractTaskData> _output;
	protected:
		TaskInfo DoGetInfo() const override;
		UniquePtr<Task> DoDuplicate() const override;
		void DoRunImpl() override;
		Size GetOperationsPerformedPerRun() override;
	};

} // Elpida

#endif //ELPIDA_DYNAMICMATRIXMULTIPLICATIONTASK_HPP
