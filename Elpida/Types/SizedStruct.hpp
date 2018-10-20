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
 * SizedStruct.hpp
 *
 *  Created on: 20 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_TYPES_SIZEDSTRUCT_HPP_
#define ELPIDA_TYPES_SIZEDSTRUCT_HPP_

#include "Elpida/Types/Primitives.hpp"
#include "Elpida/Types/Integer.hpp"

namespace Elpida
{

	template<Size Bytes>
	struct SizedStruct
	{
			Int8 value[Bytes];
	};

}  // namespace Elpida

#endif /* ELPIDA_TYPES_SIZEDSTRUCT_HPP_ */
