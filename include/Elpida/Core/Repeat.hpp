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
// Created by klapeto on 10/9/2023.
//

#ifndef ELPIDA_INCLUDE_ELPIDA_CORE_REPEAT_HPP
#define ELPIDA_INCLUDE_ELPIDA_CORE_REPEAT_HPP


#define REPEAT_5(x) x;x;x;x;x
#define REPEAT_10(x) REPEAT_5(x);REPEAT_5(x)
#define REPEAT_50(x) REPEAT_10(x);REPEAT_10(x);REPEAT_10(x);REPEAT_10(x);REPEAT_10(x)
#define REPEAT_100(x) REPEAT_50(x);REPEAT_50(x)
#define REPEAT_500(x) REPEAT_100(x);REPEAT_100(x);REPEAT_100(x);REPEAT_100(x);REPEAT_100(x)
#define REPEAT_1000(x) REPEAT_500(x);REPEAT_500(x)
#define REPEAT_5000(x) REPEAT_1000(x);REPEAT_1000(x);REPEAT_1000(x);REPEAT_1000(x);REPEAT_1000(x)
#define REPEAT_10000(x) REPEAT_5000(x);REPEAT_5000(x)

#endif //ELPIDA_INCLUDE_ELPIDA_CORE_REPEAT_HPP
