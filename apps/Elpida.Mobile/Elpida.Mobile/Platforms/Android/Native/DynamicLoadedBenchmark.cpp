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

#include "DynamicLoadedBenchmark.hpp"

#include <Elpida/Core/Config.hpp>
#include <Elpida/Core/ModuleExports.hpp>
#include <dlfcn.h>

#include "Elpida/Core/ElpidaException.hpp"

ELPIDA_CREATE_BENCHMARK_GROUP_FUNC();

namespace Elpida
{
	Benchmark* DynamicLoadedBenchmark::GetBenchmark(uint64_t index)
	{
		if (!_benchmarkGroup)
		{
			auto func = reinterpret_cast<std::unique_ptr<BenchmarkGroup>(*)()>(dlsym(_handle, "DoCreateBenchmarkGroup"));
			if (func == nullptr)
			{
			    auto err = dlerror();
				throw ElpidaException("Failed to resolve benchmark group creation function ", err);
			}
			_benchmarkGroup = func();
		}
		if (index >= _benchmarkGroup->GetBenchmarks().size())
		{
			throw std::runtime_error("Invalid benchmark index. Group Size: " + std::to_string(_benchmarkGroup->GetBenchmarks().size()));
		}
		return _benchmarkGroup->GetBenchmarks()[index].get();
	}

	DynamicLoadedBenchmark::DynamicLoadedBenchmark(const char* fileName)
	{
		_handle = dlopen(fileName, RTLD_NOW);
		if (_handle == nullptr)
		{
			throw std::runtime_error(dlerror());
		}
	}

	DynamicLoadedBenchmark::~DynamicLoadedBenchmark()
	{
		_benchmarkGroup.release();
		if (_handle != nullptr)
		{
			dlclose(_handle);
		}
	}
}