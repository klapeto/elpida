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
// Created by klapeto on 1/3/2023.
//

#ifndef ELPIDA_BENCHMARKGROUP_HPP_
#define ELPIDA_BENCHMARKGROUP_HPP_

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Benchmark.hpp"

namespace Elpida
{
	class BenchmarkGroup final
	{
	 public:
		[[nodiscard]]
		const String& GetName() const
		{
			return _name;
		}

		[[nodiscard]]
		const Vector<UniquePtr<Benchmark>>& GetBenchmarks() const
		{
			return _benchmarks;
		}

		BenchmarkGroup(String name, Vector<UniquePtr<Benchmark>>&& benchmarks)
			:  _benchmarks(std::move(benchmarks)), _name(std::move(name))
		{

		}
		BenchmarkGroup(BenchmarkGroup&&) noexcept = default;
		BenchmarkGroup(const BenchmarkGroup&) = default;
		BenchmarkGroup& operator=(BenchmarkGroup&&) noexcept = default;
		BenchmarkGroup& operator=(const BenchmarkGroup&) = default;
		~BenchmarkGroup() = default;
	 private:
		Vector<UniquePtr<Benchmark>> _benchmarks;
		String _name;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKGROUP_HPP_
