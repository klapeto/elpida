//
// Created by klapeto on 12/3/2023.
//

#ifndef ELPIDA_RESULTSFORMATTER_HPP_
#define ELPIDA_RESULTSFORMATTER_HPP_

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/BenchmarkResult.hpp"

namespace Elpida
{
	class ResultFormatter
	{
	 public:
		[[nodiscard]]
		virtual String ConvertToString(const BenchmarkResult& result) const = 0;

		ResultFormatter() = default;
		virtual ~ResultFormatter() = default;
	};

} // Elpida

#endif //ELPIDA_RESULTSFORMATTER_HPP_
