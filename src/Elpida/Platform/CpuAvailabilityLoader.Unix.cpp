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

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/CpuAvailabilityLoader.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace Elpida
{
	/// Number of CPUs in a CPU list such as "0-3,6,7"; -1 if it can't be parsed
	static int CountCpuList(const std::string& list)
	{
		int count = 0;
		std::istringstream ranges(list);
		std::string range;
		while (std::getline(ranges, range, ','))
		{
			auto dash = range.find('-');
			try
			{
				if (dash == std::string::npos)
				{
					(void)std::stoi(range);
					count += 1;
				}
				else
				{
					count += std::stoi(range.substr(dash + 1)) - std::stoi(range.substr(0, dash)) + 1;
				}
			}
			catch (const std::exception&)
			{
				return -1;
			}
		}
		return count;
	}

	static std::string ReadFirstLine(const std::string& path)
	{
		std::ifstream file(path);
		std::string line;
		if (!file || !std::getline(file, line))
		{
			return {};
		}
		return line;
	}

	/// The CPUs the calling thread may run on: Cpus_allowed_list of /proc/self/task/<tid>/status, -1 if unreadable.
	/// Not sched_getaffinity(): on recent Android kernels it also leaves out the CPUs the kernel has paused (idle big
	/// cores; they stay online and come back under load), which made 6 of 8 cores look restricted on an idle phone.
	static int CountAllowedCpus()
	{
		std::ifstream file("/proc/self/task/" + std::to_string((long)syscall(SYS_gettid)) + "/status");
		std::string line;
		const std::string key = "Cpus_allowed_list:";
		while (file && std::getline(file, line))
		{
			if (line.compare(0, key.size(), key) == 0)
			{
				auto start = line.find_first_not_of(" \t", key.size());
				return start == std::string::npos ? -1 : CountCpuList(line.substr(start));
			}
		}

		// Fallback (may undercount paused CPUs, see above)
		cpu_set_t set;
		CPU_ZERO(&set);
		if (sched_getaffinity(0, sizeof(set), &set) == 0)
		{
			return CPU_COUNT(&set);
		}
		return -1;
	}

#if !defined(__ANDROID__)
	/// The cgroup directory and its ancestors, up to the mount root: a limit anywhere above applies too. In a
	/// container the path in /proc/self/cgroup may not exist under the container's mount, but its root does.
	static std::vector<std::string> CgroupDirectories(const std::string& mount, std::string path)
	{
		std::vector<std::string> directories;
		while (true)
		{
			directories.push_back(mount + path);
			if (path.empty() || path == "/")
			{
				break;
			}
			auto slash = path.find_last_of('/');
			path = slash == 0 || slash == std::string::npos ? "/" : path.substr(0, slash);
		}
		return directories;
	}

	static double MinQuota(double current, double quota)
	{
		return quota > 0.0 && (current <= 0.0 || quota < current) ? quota : current;
	}

	/// The smallest CPU quota on the way to the root, in CPUs; -1 if none. cgroup v2: cpu.max ("max 100000" or
	/// "<quota> <period>"); v1: cpu.cfs_quota_us (-1 = none) / cpu.cfs_period_us. Docker --cpus and systemd CPUQuota=
	/// set these.
	static double LoadCgroupQuota()
	{
		std::ifstream cgroups("/proc/self/cgroup");
		std::string line;
		double quota = -1.0;
		while (cgroups && std::getline(cgroups, line))
		{
			// "<id>:<controllers>:<path>"; v2 is "0::<path>"
			auto first = line.find(':');
			auto second = first == std::string::npos ? std::string::npos : line.find(':', first + 1);
			if (second == std::string::npos)
			{
				continue;
			}
			auto controllers = line.substr(first + 1, second - first - 1);
			auto path = line.substr(second + 1);

			if (controllers.empty())
			{
				for (auto& directory : CgroupDirectories("/sys/fs/cgroup", path))
				{
					std::istringstream max(ReadFirstLine(directory + "/cpu.max"));
					std::string limit;
					double period = 0.0;
					if (max >> limit >> period && limit != "max" && period > 0.0)
					{
						try
						{
							quota = MinQuota(quota, std::stod(limit) / period);
						}
						catch (const std::exception&)
						{
						}
					}
				}
			}
			else if (("," + controllers + ",").find(",cpu,") != std::string::npos)
			{
				for (auto mount : { "/sys/fs/cgroup/cpu,cpuacct", "/sys/fs/cgroup/cpu" })
				{
					for (auto& directory : CgroupDirectories(mount, path))
					{
						try
						{
							auto limit = std::stod(ReadFirstLine(directory + "/cpu.cfs_quota_us"));
							auto period = std::stod(ReadFirstLine(directory + "/cpu.cfs_period_us"));
							if (limit > 0.0 && period > 0.0)
							{
								quota = MinQuota(quota, limit / period);
							}
						}
						catch (const std::exception&)
						{
						}
					}
				}
			}
		}
		return quota;
	}
#endif

	CpuAvailability CpuAvailabilityLoader::Load()
	{
		CpuAvailability availability;
		availability.allowed = CountAllowedCpus();
		availability.online = CountCpuList(ReadFirstLine("/sys/devices/system/cpu/online"));
#if !defined(__ANDROID__)
		// Not on Android: apps get cpu shares and uclamp there, not quotas, and a false positive would block every
		// upload from that device
		availability.quota = LoadCgroupQuota();
#endif
		return availability;
	}
} // Elpida

#endif // defined(ELPIDA_UNIX)
