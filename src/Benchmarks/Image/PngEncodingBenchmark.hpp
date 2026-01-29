/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
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

//
// Created by klapeto on 2/3/2023.
//

#ifndef _PNGENCODINGDECODINGBENCHMARK_HPP_
#define _PNGENCODINGDECODINGBENCHMARK_HPP_

#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{

	class PngEncodingBenchmark : public Benchmark
	{
	 public:
		[[nodiscard]]
		std::vector<TaskConfiguration> GetRequiredConfiguration() const override;

		PngEncodingBenchmark() = default;
		~PngEncodingBenchmark() override = default;
	 protected:

		[[nodiscard]]
		std::vector<std::unique_ptr<Task>> GetTasks(BenchmarkRunContext& context) const override;

		void DoGetBenchmarkInfo(String& name, String& description, size_t& coreTaskIndex,
				std::vector<TaskInfo>& taskInfos) const override;
	};

} // Elpida

#endif //_PNGENCODINGDECODINGBENCHMARK_HPP_
