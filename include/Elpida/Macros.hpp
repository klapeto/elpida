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
// Created by klapeto on 31/1/21.
//

#ifndef INCLUDE_ELPIDA_MACROS_HPP
#define INCLUDE_ELPIDA_MACROS_HPP

#define REPEAT_5(x) x;x;x;x
#define REPEAT_10(x) REPEAT_5(x);REPEAT_5(x)
#define REPEAT_50(x) REPEAT_10(x);REPEAT_10(x);REPEAT_10(x);REPEAT_10(x);REPEAT_10(x)
#define REPEAT_100(x) REPEAT_50(x);REPEAT_50(x)

#endif //INCLUDE_ELPIDA_MACROS_HPP
