//
// Created by klapeto on 1/3/2023.
//

#ifndef _BENCHMARKGROUP_HPP_
#define _BENCHMARKGROUP_HPP_

#include <string>
#include <vector>
#include <memory>

#include "Elpida/Benchmark.hpp"

namespace Elpida
{
	class BenchmarkGroup final
	{
		[[nodiscard]]
		const std::string& GetName() const
		{
			return _name;
		}

		[[nodiscard]]
		const std::vector<std::unique_ptr<const Benchmark>>& GetBenchmarks() const
		{
			return _benchmarks;
		}

		~BenchmarkGroup() = default;
	 protected:
		BenchmarkGroup(std::string name)
			: _name(std::move(name))
		{
		}
		std::vector<std::unique_ptr<const Benchmark>> _benchmarks;
	 private:
		std::string _name;
	};

} // Elpida

#endif //_BENCHMARKGROUP_HPP_
