//
// Created by klapeto on 1/3/2023.
//

#ifndef ELPIDA_BENCHMARKGROUP_HPP_
#define ELPIDA_BENCHMARKGROUP_HPP_

#include "Elpida/String.hpp"
#include "Elpida/Vector.hpp"
#include "Elpida/UniquePtr.hpp"
#include "Elpida/Benchmark.hpp"

namespace Elpida
{
	class BenchmarkGroup final
	{
		[[nodiscard]]
		const String& GetName() const
		{
			return _name;
		}

		[[nodiscard]]
		const Vector<UniquePtr<const Benchmark>>& GetBenchmarks() const
		{
			return _benchmarks;
		}

		~BenchmarkGroup() = default;
	 protected:
		BenchmarkGroup(String name)
			: _name(std::move(name))
		{
		}
		Vector<UniquePtr<const Benchmark>> _benchmarks;
	 private:
		String _name;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKGROUP_HPP_
