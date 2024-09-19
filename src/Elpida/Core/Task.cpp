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
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Core/Task.hpp"

#include "Elpida/Core/EnvironmentInfo.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{

	Duration Task::Run()
	{
		auto a = Timer::now();

		DoRun();

		auto b = Timer::now();

		return PostProcessDuration(b - a - _benchmarkRunContext->get().GetEnvironmentInfo().GetOverheadsInfo().GetNowOverhead());
	}

	UniquePtr<Task> Task::Duplicate() const
	{
		auto task = DoDuplicate();
		task->SetMeasured(ShouldBeMeasured());
		task->SetBenchmarkRunContext(_benchmarkRunContext->get());
		return task;
	}


	Duration Task::PostProcessDuration(const Duration& duration) const
	{
		return duration;
	}

	void Task::SetBenchmarkRunContext(const BenchmarkRunContext& runContext)
	{
		_benchmarkRunContext = runContext;
	}

} // Elpida