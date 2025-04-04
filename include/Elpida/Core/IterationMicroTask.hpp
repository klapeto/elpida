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
// Created by klapeto on 22/9/2024.
//

#ifndef ELPIDA_ITERATIONMICROTASK_HPP
#define ELPIDA_ITERATIONMICROTASK_HPP

#include <cmath>
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/Task.hpp"
#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/Iterations.hpp"

namespace Elpida
{

	class IterationMicroTask: public Task
	{
	public:
		Duration Run() final;

		IterationMicroTask() = default;
		~IterationMicroTask() override = default;
	protected:
		void DoRun() final;
		virtual void DoRun(Iterations iterations) = 0;
		virtual Size GetOperationsPerformedPerRun() = 0;
		virtual Duration GetExecutionMinimumDuration();
		virtual void OnBeforeRun(Iterations iterations);
	};

} // Elpida

#endif //ELPIDA_ITERATIONMICROTASK_HPP
