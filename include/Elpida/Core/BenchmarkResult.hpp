//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARKRESULT_HPP
#define ELPIDA_BENCHMARKRESULT_HPP

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/TaskResult.hpp"

namespace Elpida
{
	class BenchmarkResult final
	{
	 public:

		[[nodiscard]]
		double GetResult() const
		{
			return _result;
		}

		explicit BenchmarkResult(double result)
				: _result(result)
		{
		}

		BenchmarkResult(const BenchmarkResult&) = delete;
		BenchmarkResult(BenchmarkResult&&) = default;
		~BenchmarkResult() = default;
	 private:
		double _result;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKRESULT_HPP
