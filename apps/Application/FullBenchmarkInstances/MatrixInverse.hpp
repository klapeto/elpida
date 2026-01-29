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

#ifndef ELPIDA_MATRIXINVERSE_HPP
#define ELPIDA_MATRIXINVERSE_HPP

#include "Controllers/FullBenchmarkInstance.hpp"

namespace Elpida::Application
{
	class MatrixInverse : public FullBenchmarkInstance
	{
	public:
		MatrixInverse(const std::string& uuid,
				Score baseScore,
				bool multiThreaded,
				const BenchmarkModel& benchmark,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const MemoryInfoModel& memoryInfoModel,
				const BenchmarkRunConfigurationModel& runConfigurationModel,
				BenchmarkExecutionService& executionService);

		MatrixInverse(const MatrixInverse&) = delete;
		MatrixInverse(MatrixInverse&&) noexcept = default;
		MatrixInverse& operator=(const MatrixInverse&) = delete;
		MatrixInverse& operator=(MatrixInverse&&) noexcept = delete;
		~MatrixInverse() override = default;
	protected:
		void Configure() override;
	};

} // Elpida::Application

#endif //ELPIDA_MATRIXINVERSE_HPP
