/*
 *  Copyright (c) 2026  Ioannis Panagiotopoulos
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

#ifndef ELPIDA_PLATFORM_CPUAVAILABILITYLOADER_HPP_
#define ELPIDA_PLATFORM_CPUAVAILABILITYLOADER_HPP_

#include "Elpida/Core/String.hpp"

namespace Elpida
{
	/// How much of the machine's CPUs this process, and the benchmark processes it starts (which inherit it), may use
	/// (docs/development/cpu-availability.md). Results from a restricted run are not uploaded: they are lower than the
	/// CPU can do and the uploaded topology shows only the allowed cores.
	struct CpuAvailability
	{
		/// The CPUs this thread may run on (affinity, cpuset, job object affinity); -1 if unknown
		int allowed = -1;

		/// The online CPUs; -1 if unknown. Offline CPUs don't count: on desktop that's SMT or cores disabled by the
		/// user (a real configuration, visible in the topology); on Android, parked cores
		int online = -1;

		/// A CPU time quota in CPUs (2.5 = two and a half CPUs' worth of time: cgroup cpu.max / cpu.cfs_quota_us,
		/// Windows job object CPU rate cap); -1 if none or unknown
		double quota = -1.0;

		/// Fewer allowed CPUs than online ones
		[[nodiscard]]
		bool IsAffinityRestricted() const
		{
			return allowed > 0 && online > 0 && allowed < online;
		}

		/// A quota below the CPUs the process may run on
		[[nodiscard]]
		bool IsQuotaRestricted() const
		{
			auto cpus = allowed > 0 ? allowed : online;
			return quota > 0.0 && cpus > 0 && quota < cpus - 0.01;
		}

		[[nodiscard]]
		bool IsRestricted() const
		{
			return IsAffinityRestricted() || IsQuotaRestricted();
		}
	};

	class CpuAvailabilityLoader final
	{
	 public:
		/// For the calling thread (on Linux the affinity is per thread; child processes inherit the creating thread's)
		[[nodiscard]]
		static CpuAvailability Load();
	};

} // Elpida

#endif //ELPIDA_PLATFORM_CPUAVAILABILITYLOADER_HPP_
