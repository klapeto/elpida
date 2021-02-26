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
// Created by klapeto on 30/1/21.
//

#ifndef SRC_ELPIDA_SERVICEPROVIDER_HPP
#define SRC_ELPIDA_SERVICEPROVIDER_HPP

namespace Elpida
{
	class CpuInfo;
	class MemoryInfo;
	class SystemTopology;
	class OsInfo;
	class TimingInfo;
	class Logger;

	class ServiceProvider final
	{
	public:

		[[nodiscard]]
		const CpuInfo& getCpuInfo() const
		{
			return _cpuInfo;
		}

		[[nodiscard]]
		const MemoryInfo& getMemoryInfo() const
		{
			return _memoryInfo;
		}

		[[nodiscard]]
		const SystemTopology& getSystemTopology() const
		{
			return _systemTopology;
		}

		[[nodiscard]]
		const OsInfo& getOsInfo() const
		{
			return _osInfo;
		}

		[[nodiscard]]
		const TimingInfo& getTimingInfo() const
		{
			return _timingInfo;
		}

		[[nodiscard]]
		Logger& getLogger() const
		{
			return _logger;
		}

		ServiceProvider(const CpuInfo& cpuInfo,
			const MemoryInfo& memoryInfo,
			const SystemTopology& systemTopology,
			const OsInfo& osInfo,
			const TimingInfo& timingInfo,
			Logger& logger);
		~ServiceProvider() = default;
	private:
		const CpuInfo& _cpuInfo;
		const MemoryInfo& _memoryInfo;
		const SystemTopology& _systemTopology;
		const OsInfo& _osInfo;
		const TimingInfo& _timingInfo;
		Logger& _logger;
	};
}

#endif //SRC_ELPIDA_SERVICEPROVIDER_HPP
