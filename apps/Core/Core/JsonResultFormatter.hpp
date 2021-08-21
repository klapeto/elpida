/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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
// Created by klapeto on 21/6/20.
//

#ifndef APPS_QT_CORE_JSONRESULTFORMATTER_HPP
#define APPS_QT_CORE_JSONRESULTFORMATTER_HPP

#include "Core/Abstractions/ResultFormatter.hpp"

namespace Elpida
{
	class SystemTopology;
	class CpuInfo;
	class OsInfo;
	class MemoryInfo;
	class TimingInfo;

	class JsonResultFormatter : public ResultFormatter
	{
	public:

		[[nodiscard]]
		const std::string& getFileExtension() const override
		{
			return _fileExtension;
		}

		[[nodiscard]]
		std::string serialize(std::vector<BenchmarkResult>& results) const override;

		JsonResultFormatter(const SystemTopology& systemTopology,
			const CpuInfo& cpuInfo,
			const OsInfo& osInfo,
			const MemoryInfo& memoryInfo,
			const TimingInfo& timingInfo);
		~JsonResultFormatter() override = default;
	private:
		const SystemTopology& _systemTopology;
		const CpuInfo& _cpuInfo;
		const OsInfo& _osInfo;
		const MemoryInfo& _memoryInfo;
		const TimingInfo& _timingInfo;

		std::string _fileExtension = "json";
	};

}
#endif //APPS_QT_CORE_JSONRESULTFORMATTER_HPP
