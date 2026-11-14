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

#include "CpuRestriction.hpp"

#include "Elpida/Core/Config.hpp"

#include <iomanip>
#include <sstream>

namespace Elpida::Application
{
	std::optional<CpuAvailability> CpuRestriction::Check()
	{
		try
		{
			auto availability = CpuAvailabilityLoader::Load();
			if (availability.IsRestricted())
			{
				return availability;
			}
		}
		catch (const std::exception&)
		{
			// Unknown: don't block
		}
		return std::nullopt;
	}

	const CpuAvailability& CpuRestriction::Worse(const CpuAvailability& a, const CpuAvailability& b)
	{
		auto effective = [](const CpuAvailability& availability)
		{
			double cpus = availability.IsAffinityRestricted() ? availability.allowed : availability.online;
			if (availability.IsQuotaRestricted() && availability.quota < cpus)
			{
				cpus = availability.quota;
			}
			return cpus;
		};
		return effective(b) < effective(a) ? b : a;
	}

	std::string CpuRestriction::Describe(const CpuAvailability& availability)
	{
		std::ostringstream text;
		if (availability.IsAffinityRestricted())
		{
			text << "Elpida may run on only " << availability.allowed << " of the " << availability.online
				 << " logical processors. Something set its processor affinity: "
#if defined(ELPIDA_WINDOWS)
				 << "for example \"Set affinity\" in Task Manager, \"start /affinity\", or a tool that manages it.";
#else
				 << "for example taskset, a cpuset (Docker --cpuset-cpus, systemd AllowedCPUs=), or a tool that "
					"manages it.";
#endif
		}
		if (availability.IsQuotaRestricted())
		{
			if (availability.IsAffinityRestricted())
			{
				text << " Also, a";
			}
			else
			{
				text << "A";
			}
			text << " CPU time limit gives Elpida only " << std::fixed << std::setprecision(1) << availability.quota
				 << " CPUs' worth of time"
#if defined(ELPIDA_WINDOWS)
				 << " (a job object CPU rate limit, for example a container's --cpus).";
#else
				 << " (a cgroup CPU quota, for example Docker --cpus or systemd CPUQuota=).";
#endif
		}
		return text.str();
	}

	std::string CpuRestriction::GetWarning(const CpuAvailability& availability, bool uploadEnabled)
	{
		std::ostringstream text;
		text << Describe(availability) << "\n\nThe results would be lower than this computer can do";
		if (uploadEnabled)
		{
			text << ", so they will not be uploaded";
		}
		text << ". If you think this is wrong, please report it to us: " << ReportIssueUrl;
		return text.str();
	}

	std::string CpuRestriction::GetNotUploadedMessage(const CpuAvailability& availability)
	{
		std::ostringstream text;
		text << "The results were not uploaded: during the run, the CPU was restricted. " << Describe(availability)
			 << "\n\nIf you think this is wrong, please report it to us: " << ReportIssueUrl;
		return text.str();
	}
} // Elpida::Application
