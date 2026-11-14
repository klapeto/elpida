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

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/CpuAvailabilityLoader.hpp"

#include <windows.h>

namespace Elpida
{
	static int CountBits(DWORD_PTR mask)
	{
		int count = 0;
		for (; mask != 0; mask &= mask - 1)
		{
			++count;
		}
		return count;
	}

	/// The job object's hard CPU rate cap, in CPUs; -1 if none. A cap is a percentage of the whole machine (Docker
	/// --cpus for Windows containers, or any job with JOB_OBJECT_CPU_RATE_CONTROL_HARD_CAP). Windows 8 and later; on
	/// older versions the query fails and there's no cap. The affinity limit of a job shows in the process affinity.
	static double LoadJobQuota(int machineCpus)
	{
		// JOBOBJECT_CPU_RATE_CONTROL_INFORMATION and JobObjectCpuRateControlInformation (15) aren't declared with
		// _WIN32_WINNT 0x0501
		struct CpuRateControlInformation
		{
			DWORD controlFlags;
			union
			{
				DWORD cpuRate;
				DWORD weight;
				struct
				{
					WORD minRate;
					WORD maxRate;
				} minMax;
			};
		};
		constexpr int jobObjectCpuRateControlInformation = 15;
		constexpr DWORD enable = 0x1, hardCap = 0x4, minMaxRate = 0x10;

		CpuRateControlInformation information{};
		if (!QueryInformationJobObject(nullptr, static_cast<JOBOBJECTINFOCLASS>(jobObjectCpuRateControlInformation),
				&information, sizeof(information), nullptr))
		{
			return -1.0;    // not in a job, or Windows before 8
		}
		if ((information.controlFlags & enable) == 0 || machineCpus <= 0)
		{
			return -1.0;
		}

		// Rates are in 1/100 of a percent of the machine (10000 = all of it). Weight based control is not a cap.
		DWORD rate = 0;
		if ((information.controlFlags & hardCap) != 0)
		{
			rate = information.cpuRate;
		}
		else if ((information.controlFlags & minMaxRate) != 0)
		{
			rate = information.minMax.maxRate;
		}
		if (rate == 0 || rate >= 10000)
		{
			return -1.0;
		}
		return machineCpus * (rate / 10000.0);
	}

	CpuAvailability CpuAvailabilityLoader::Load()
	{
		CpuAvailability availability;

		// Compared within the process' processor group: on machines with more than 64 logical processors a process
		// starts in one group (Windows 10) and that's not a restriction by the user. When its threads run in several
		// groups (Windows 11 default), both masks are 0: unknown, not restricted.
		DWORD_PTR processMask = 0, systemMask = 0;
		if (GetProcessAffinityMask(GetCurrentProcess(), &processMask, &systemMask) && systemMask != 0)
		{
			availability.allowed = CountBits(processMask & systemMask);
			availability.online = CountBits(systemMask);
		}

		// The whole machine for the job's rate, which is relative to all processors (all groups, Windows 7+)
		int machineCpus = availability.online;
		using GetActiveProcessorCountFunction = DWORD (WINAPI*)(WORD);
		auto kernel32 = GetModuleHandleW(L"kernel32.dll");
		auto getActiveProcessorCount = kernel32 == nullptr ? nullptr
				: reinterpret_cast<GetActiveProcessorCountFunction>(
						reinterpret_cast<void*>(GetProcAddress(kernel32, "GetActiveProcessorCount")));
		if (getActiveProcessorCount != nullptr)
		{
			constexpr WORD allProcessorGroups = 0xffff;
			auto count = getActiveProcessorCount(allProcessorGroups);
			if (count > 0)
			{
				machineCpus = static_cast<int>(count);
			}
		}
		availability.quota = LoadJobQuota(machineCpus);

		return availability;
	}
} // Elpida

#endif // defined(ELPIDA_WINDOWS)
