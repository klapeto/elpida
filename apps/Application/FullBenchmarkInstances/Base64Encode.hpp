/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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

#ifndef ELPIDA_BASE64ENCODE_HPP
#define ELPIDA_BASE64ENCODE_HPP

#include "Controllers/FullBenchmarkInstance.hpp"

namespace Elpida::Application
{
	class Base64Encode : public FullBenchmarkInstance
	{
	public:
		Base64Encode(const std::string& uuid,
				Score baseScore,
				bool multiThreaded,
				const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				const BenchmarkRunConfigurationModel& runConfigurationModel,
				BenchmarkExecutionService& executionService);

		Base64Encode(const Base64Encode&) = delete;
		Base64Encode(Base64Encode&&) noexcept = default;
		Base64Encode& operator=(const Base64Encode&) = delete;
		Base64Encode& operator=(Base64Encode&&) noexcept = delete;
		~Base64Encode() override = default;
	protected:
		void Configure() override;
		ConcurrencyMode GetMultiThreadConcurrencyMode() const override;
	};

} // Elpida::Application

#endif //ELPIDA_BASE64ENCODE_HPP
