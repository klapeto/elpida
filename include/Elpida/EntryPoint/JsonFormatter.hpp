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
