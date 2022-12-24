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

#ifndef SRC_ELPIDA_SYSTEMINFO_TIMINGINFO_HPP
#define SRC_ELPIDA_SYSTEMINFO_TIMINGINFO_HPP

#include <cstdint>
#include <chrono>
#include <mutex>

#include "Elpida/Utilities/Duration.hpp"
#include "Elpida/Engine/Runner/MicroTaskRunner.hpp"

namespace Elpida
{
	class SystemTopology;

	class ProcessorNode;

	class TimingInfo final
	{
	public:
		[[nodiscard]]
		bool isCalculated() const
		{
			return _calculated;
		}

		[[nodiscard]]
		const Duration& getNowOverhead() const
		{
			return _nowOverhead;
		}

		[[nodiscard]]
		const Duration& getSleepOverhead() const
		{
			return _sleepOverhead;
		}

		[[nodiscard]]
		const Duration& getTargetTime() const
		{
			return _targetTime;
		}

		[[nodiscard]]
		const Duration& getLockOverhead() const
		{
			return _lockOverhead;
		}

		[[nodiscard]]
		const Duration& getWakeupOverhead() const
		{
			return _wakeupOverhead;
		}

		[[nodiscard]]
		const Duration& getNotifyOverhead() const
		{
			return _notifyOverhead;
		}

		[[nodiscard]]
		const Duration& getLoopOverhead() const
		{
			return _loopOverhead;
		}

		[[nodiscard]]
		const Duration& getJoinOverhead() const
		{
			return _joinOverhead;
		}


		[[nodiscard]]
		bool isTargetTimeFallback() const
		{
			return _targetTime >= FallbackTargetTime;
		}

		void setTargetTime(Duration duration)
		{
			_targetTime = duration;
		}

		void reCalculate(const SystemTopology& systemTopology);

		template<typename TCallable>
		size_t calculateIterationsForTime(const Duration& targetDuration, TCallable callable) const
		{
			auto duration = MicroTaskRunner::measure(_targetTime, 1, _nowOverhead, _loopOverhead, callable);

			return targetDuration / duration;
		}


		TimingInfo() = default;

		~TimingInfo() = default;

	private:
		const Duration FallbackTargetTime = MilliSeconds(250);
		static constexpr Duration PossibleTargetTimes[] = {
				MilliSeconds(100),
				MilliSeconds(200),
		};

		Duration _targetTime = FallbackTargetTime;
		Duration _loopOverhead = Duration::zero();
		Duration _lockOverhead = Duration::zero();
		Duration _notifyOverhead = Duration::zero();
		Duration _wakeupOverhead = Duration::zero();;
		Duration _nowOverhead = Duration::zero();
		Duration _sleepOverhead = Duration::zero();
		Duration _joinOverhead = Duration::zero();
		bool _calculated = false;

		void calculateNowOverhead();

		void calculateSleepOverhead();

		void calculateLockUnlockOverhead();

		void calculateNotifyOverhead();

		void calculateWakeupOverhead();

		void calculateLoopOverhead();

		void calculateJoinOverhead();

		void testTime(std::mutex& mutex, Duration time, bool& cancel);
	};
}

#endif //SRC_ELPIDA_SYSTEMINFO_TIMINGINFO_HPP
