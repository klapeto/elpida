/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 30/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_DATA_DATAPROPERTIESTRANSFORMER_HPP
#define INCLUDE_ELPIDA_ENGINE_DATA_DATAPROPERTIESTRANSFORMER_HPP

#include <unordered_map>
#include <string>

namespace Elpida
{
	class DataPropertiesTransformer
	{
	public:
		[[nodiscard]] virtual std::unordered_map<std::string, double> transform(size_t originalSize,
			const std::unordered_map<std::string, double>& originalProperties,
			size_t targetSize) const = 0;

		DataPropertiesTransformer() = default;
		virtual ~DataPropertiesTransformer() = default;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_DATA_DATAPROPERTIESTRANSFORMER_HPP
