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

//
// Created by klapeto on 20/9/2024.
//

#ifndef ELPIDA_GENERATERANDOMDATATASK_HPP
#define ELPIDA_GENERATERANDOMDATATASK_HPP

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Task.hpp"

namespace Elpida
{

	class GenerateRandomDataTask : public Task
	{
	public:
		void Prepare(SharedPtr<AbstractTaskData> inputData) override;
		SharedPtr<AbstractTaskData> Finalize() override;
		ConcurrencyMode GetAllowedConcurrency() const override;
		Size GetProcessedDataSize() const override;

		explicit GenerateRandomDataTask(std::size_t size);
		~GenerateRandomDataTask() override = default;
	protected:
		void DoRun() override;
		std::unique_ptr<Task> DoDuplicate() const override;
		TaskInfo DoGetInfo() const override;
	private:
		SharedPtr<AbstractTaskData> _outputData;
		std::size_t _size;
	};

} // Elpida

#endif //ELPIDA_GENERATERANDOMDATATASK_HPP
