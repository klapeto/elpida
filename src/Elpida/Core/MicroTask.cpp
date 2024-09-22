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

#include "Elpida/Core/MicroTask.hpp"

#include "Elpida/Core/EnvironmentInfo.hpp"
#include "Elpida/Core/TimingUtilities.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

namespace Elpida
{
	Duration MicroTask::Run()
	{
		auto& context = _benchmarkRunContext.value().get();
		auto& overheads = context.GetEnvironmentInfo().GetOverheadsInfo();

		Duration minimumDuration = std::max(GetExecutionMinimumDuration(), context.GetMinimumMicroTaskDuration());
		Duration nowOverhead = overheads.GetNowOverhead();
		Duration loopOverhead = overheads.GetLoopOverhead();

		Duration currentDuration;

		if (ShouldBeMeasured())
		{
			bool doContinue = false;
			std::mutex mutex;
			std::condition_variable conditionVariable;

			auto th = std::thread([&]()
			{
				// avoid forced schedule to the same thread
				context.GetEnvironmentInfo().GetTopologyInfo().ClearThreadPinning();
				{
					// wait until measure thread tells us to sleep.
					std::unique_lock<std::mutex> lock(mutex);
					while (!doContinue)
					{
						conditionVariable.wait(lock);
					}
				}

				std::this_thread::sleep_for(minimumDuration);

				_keepGoing = false;
			});

			// Warmup
			_keepGoing = false;
			DoRunImpl();
			_keepGoing = true;

			{
				// Tell control thread to continue and go to sleep
				std::unique_lock<std::mutex> lock(mutex);
				doContinue = true;
				conditionVariable.notify_all();
			}

			_iterations = 0;
			auto start = Timer::now();
			DoRunImpl();
			auto end = Timer::now();

			th.join();

			currentDuration = ToDuration(end - start) - nowOverhead - (loopOverhead * _iterations);
			currentDuration = PostProcessDuration(currentDuration);
		}
		else
		{
			_keepGoing = false;
			auto start = Timer::now();
			DoRunImpl();
			auto end = Timer::now();

			currentDuration = ToDuration(end - start) - nowOverhead - (loopOverhead * _iterations);
			currentDuration = PostProcessDuration(currentDuration);
		}

		currentDuration = (currentDuration / static_cast<double>(_iterations)) /
						  static_cast<double>(GetOperationsPerformedPerRun());

		return currentDuration;
	}

	void MicroTask::DoRun()
	{

	}

	Duration MicroTask::GetExecutionMinimumDuration()
	{
		return Seconds(0.1);
	}

	MicroTask::MicroTask()
			:_iterations(0), _keepGoing(false)
	{

	}

} // Elpida
