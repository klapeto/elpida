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
			: _name(std::move(name)), _benchmarks(std::move(benchmarks))
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
