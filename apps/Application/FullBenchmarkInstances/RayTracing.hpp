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

#ifndef ELPIDA_RAYTRACING_HPP
#define ELPIDA_RAYTRACING_HPP

#include "Controllers/FullBenchmarkInstance.hpp"

namespace Elpida::Application
{
	class RayTracing : public FullBenchmarkInstance
	{
	public:
		RayTracing(const std::string& uuid,
				Score baseScore,
				bool multiThreaded,
				const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				const BenchmarkRunConfigurationModel& runConfigurationModel,
				BenchmarkExecutionService& executionService);

		RayTracing(const RayTracing&) = delete;
		RayTracing(RayTracing&&) noexcept = default;
		RayTracing& operator=(const RayTracing&) = delete;
		RayTracing& operator=(RayTracing&&) noexcept = delete;
		~RayTracing() override = default;
	protected:
		void Configure() override;
	};

} // Elpida::Application

#endif //ELPIDA_RAYTRACING_HPP
