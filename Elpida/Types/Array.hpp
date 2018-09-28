/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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

/*
 * Array.hpp
 *
 *  Created on: 29 Ιουλ 2018
 *      Author: klapeto
 */

#ifndef SRC_UTILITIES_ARRAY_HPP_
#define SRC_UTILITIES_ARRAY_HPP_

#include <vector>

namespace Elpida
{

	template<typename T>
	using Array = std::vector<T>;

} /* namespace Elpida */

#endif /* SRC_UTILITIES_ARRAY_HPP_ */