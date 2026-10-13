/*
*  Copyright (c) 2024-2026  Ioannis Panagiotopoulos
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
// Created by klapeto on 4/10/26.
//

#ifndef ELPIDA_DYNAMICLOADEDBENCHMARK_HPP
#define ELPIDA_DYNAMICLOADEDBENCHMARK_HPP

#include <new>
#include <Elpida/Core/BenchmarkGroup.hpp>
#include <Elpida/Core/Benchmark.hpp>

namespace Elpida
{
	class DynamicLoadedBenchmark
	{
	public:
		Benchmark* GetBenchmark(uint64_t index);
		DynamicLoadedBenchmark(const char* fileName);
		DynamicLoadedBenchmark(const DynamicLoadedBenchmark&) = delete;
		DynamicLoadedBenchmark(DynamicLoadedBenchmark&&) = delete;
		DynamicLoadedBenchmark& operator=(const DynamicLoadedBenchmark&) = delete;
		DynamicLoadedBenchmark& operator=(DynamicLoadedBenchmark&&) = delete;
		~DynamicLoadedBenchmark();
	private:
		std::unique_ptr<BenchmarkGroup> _benchmarkGroup;
		void* _handle;
	};
}
#endif //ELPIDA_DYNAMICLOADEDBENCHMARK_HPP
