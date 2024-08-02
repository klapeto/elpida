//
// Created by klapeto on 24/4/2023.
//

#ifndef ELPIDA_JSONFORMATTER_HPP
#define ELPIDA_JSONFORMATTER_HPP

#include "ResultFormatter.hpp"

namespace Elpida
{

	class JsonFormatter final : public ResultFormatter
	{
	public:
		[[nodiscard]]
		String ConvertToString(const BenchmarkResult& result ,const Benchmark& benchmark) const final;

		JsonFormatter() = default;
		~JsonFormatter() final = default;
	};

} // Elpida

#endif //ELPIDA_JSONFORMATTER_HPP
