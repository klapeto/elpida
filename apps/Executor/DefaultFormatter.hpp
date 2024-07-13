//
// Created by klapeto on 12/3/2023.
//

#ifndef _DEFAULTFORMATTER_HPP_
#define _DEFAULTFORMATTER_HPP_

#include "ResultFormatter.hpp"

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/BenchmarkResult.hpp"

namespace Elpida
{

	class DefaultFormatter final : public ResultFormatter
	{
	 public:
		[[nodiscard]]
		String ConvertToString(const BenchmarkResult& result, const Benchmark& benchmark) const final;

		DefaultFormatter() = default;
		~DefaultFormatter() final = default;
	};

} // Elpida

#endif //_DEFAULTFORMATTER_HPP_
