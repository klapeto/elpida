/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021  Ioannis Panagiotopoulos
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
// Created by klapeto on 15/1/21.
//

#ifndef INCLUDE_ELPIDA_OPTIONALREFERENCE_HPP
#define INCLUDE_ELPIDA_OPTIONALREFERENCE_HPP

#include <optional>
#include <functional>

namespace Elpida {

	template<typename T>
	using OptionalReference = std::optional<std::reference_wrapper<T>>;
}

#endif //INCLUDE_ELPIDA_OPTIONALREFERENCE_HPP
