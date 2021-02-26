/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 31/1/21.
//

#include "Elpida/SystemInfo/TimingInfo.hpp"
#include "Elpida/Utilities/OsUtilities.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"
#include "Elpida/SystemInfo/ProcessorNode.hpp"
#include "Elpida/SystemInfo/SystemTopology.hpp"
#include "Elpida/Timer.hpp"
#include "Elpida/Engine/Runner/DefaultTaskRunner.hpp"

#include <thread>
#include <condition_variable>
#include <stack>
#include <functional>

namespace Elpida
{

	template<typename TCallable>
	static inline Duration calculateAverage(size_t iterations, const Duration& nowOverhead, TCallable callable)
	{
		Duration curDuration = Duration::zero();
		for (size_t i = 0; i < iterations; ++i)
		{
			auto start = Timer::now();
			callable();
			auto end = Timer::now();

			curDuration += std::max(Duration::zero(), ToDuration(end - start) - nowOverhead);
		}

		return curDuration / iterations;
	}

	void TimingInfo::calculateNowOverhead()
	{
		_nowOverhead = calculateAverage(1000, MilliSeconds(0), [](){ Timer::now(); });
	}

	void TimingInfo::reCalculate(const SystemTopology& systemTopology)
	{
		using Calculator = std::function<void()>;

		calculateNowOverhead();

		auto reversed = systemTopology.getAllProcessors();

		std::sort(reversed.begin(), reversed.end(),
			[](auto a, auto b)
			{
				return a->getOsIndex() > b->getOsIndex();
			});

		std::stack<const ProcessorNode*, std::vector<const ProcessorNode*>> processors(reversed);

		std::vector<Calculator> calculators
			{
				[this]
				{
					calculateSleepOverhead();
				},
				[this]
				{
					calculateNotifyOverhead();
				},
				[this]
				{
					calculateLockUnlockOverhead();
				},
				[this]
				{
					calculateWakeupOverhead();
				},
				[this]
				{
					calculateLoopOverhead();
				},
				[this]
				{
					calculateJoinOverhead();
				}
			};

		if (processors.size() > 1)
		{
			processors.pop();    // ignore processor 0
		}

		std::vector<std::thread> calculatingThreads;
		std::mutex mutex;
		std::condition_variable conditionVariable;

		for (auto& func: calculators)
		{
			std::unique_lock<std::mutex> lock(mutex);
			while (processors.empty())
			{
				conditionVariable.wait(lock);
			}

			auto processor = processors.top();
			processors.pop();

			calculatingThreads.emplace_back([&mutex, &conditionVariable, &processors]
				(const ProcessorNode* processorNode, const Calculator& calculator)
			{
				OsUtilities::setCurrentThreadAffinity(processorNode->getOsIndex());

				calculator();

				std::unique_lock<std::mutex> lock(mutex);
				processors.push(processorNode);
				conditionVariable.notify_all();
			}, processor, func);
		}

		for (auto& th: calculatingThreads)
		{
			if (th.joinable())
			{
				th.join();
			}
		}

		_calculated = true;
	}

	void TimingInfo::calculateSleepOverhead()
	{
		const auto millisecond = MilliSeconds(1);

		const auto func = [&millisecond]
		{
			std::this_thread::sleep_for(millisecond);
		};

		_sleepOverhead = calculateAverage(100, _nowOverhead, func) - millisecond;
	}

	void TimingInfo::calculateLockUnlockOverhead()
	{
		std::mutex mutex;
		std::condition_variable conditionVariable;

		const auto calcFunc = [&mutex]()
		{
			std::unique_lock<std::mutex> lock(mutex);
		};

		bool stop = false;
		bool started = false;

		auto thread = std::thread([&mutex, &conditionVariable, &stop, &started]()
		{
			{
				std::unique_lock<std::mutex> lock(mutex);
				started = true;
				conditionVariable.notify_all();
			}

			while (!stop)
			{
				std::unique_lock<std::mutex> lock(mutex);
			}
		});

		{
			std::unique_lock<std::mutex> lock(mutex);
			while (!started) conditionVariable.wait(lock);
		}

		_lockOverhead = calculateAverage(1000, _nowOverhead, calcFunc);

		stop = true;

		thread.join();
	}

	void TimingInfo::calculateWakeupOverhead()
	{
		std::mutex mutex;
		std::condition_variable conditionVariable;

		const auto calcFunc = [&mutex, &conditionVariable]()
		{
			std::unique_lock<std::mutex> lock(mutex);
			conditionVariable.wait(lock);
		};

		bool stop = false;

		auto thread = std::thread([&mutex, &conditionVariable, &stop]()
		{
			while (!stop)
			{
				std::unique_lock<std::mutex> lock(mutex);
				conditionVariable.notify_all();
			}
		});

		{
			std::unique_lock<std::mutex> lock(mutex);
			conditionVariable.wait(lock);
		}

		_wakeupOverhead = calculateAverage(1000, _nowOverhead, calcFunc);

		stop = true;
		thread.join();
	}

	void TimingInfo::calculateNotifyOverhead()
	{
		std::mutex mutex;
		std::condition_variable conditionVariable;

		const auto calcFunc = [&conditionVariable]()
		{
			conditionVariable.notify_all();
		};

		bool stop = false;
		bool started = false;

		auto thread = std::thread([&mutex, &conditionVariable, &stop, &started]()
		{
			{
				std::unique_lock<std::mutex> lock(mutex);
				started = true;
				conditionVariable.notify_all();
			}

			while (!stop)
			{
				std::unique_lock<std::mutex> lock(mutex);
				if (stop) return;
				conditionVariable.wait(lock);
			}
		});

		{
			std::unique_lock<std::mutex> lock(mutex);
			while (!started) conditionVariable.wait(lock);
		}

		_notifyOverhead = calculateAverage(1000, _nowOverhead, calcFunc);

		{
			std::unique_lock<std::mutex> lock(mutex);
			stop = true;
			conditionVariable.notify_all();
		}

		thread.join();
	}

	void TimingInfo::calculateJoinOverhead()
	{
		const size_t threadN = 100;

		int* x = reinterpret_cast<int*>(&x);
		volatile int** y = (volatile int**)(&x);

		const auto threadFunc = [&y]()
		{
			y = (volatile int**)*y;
		};

		std::vector<std::thread> threads;
		threads.reserve(threadN);

		for (size_t i = 0; i < threadN; ++i)
		{
			threads.emplace_back(threadFunc);
		}

		const auto calcFunc = [&threads]()
		{
			for (size_t i = 0; i < threadN; ++i)
			{
				threads[i].join();
			}
		};

		std::this_thread::yield();

		_joinOverhead = DefaultTaskRunner::measure(_nowOverhead, calcFunc) / (double)threadN;
	}

#ifdef ELPIDA_GCC
#pragma GCC optimize ("O0")
#else
#pragma optimize( "", off )
#endif
	void TimingInfo::calculateLoopOverhead()
	{
		const size_t loops = 10000;
		const size_t iterations = 1000;

		Duration curDuration = Duration::max();
		for (size_t i = 0; i < iterations; ++i)
		{
			auto start = Timer::now();
			for (size_t j = 0; j < loops; ++j);
			auto end = Timer::now();

			auto thisDuration = ToDuration(end - start);
			if (curDuration > thisDuration)
			{
				curDuration = thisDuration;
			}
		}

		curDuration -= _nowOverhead;
		_loopOverhead=  (curDuration > Duration::zero() ? curDuration : Duration::zero()) / loops;
	}
#ifdef ELPIDA_GCC
#pragma GCC reset_options
#else
#pragma optimize( "", on )
#endif

}