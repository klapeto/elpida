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

#ifndef ELPIDA_OPTIMIZATION_HPP
#define ELPIDA_OPTIMIZATION_HPP

namespace Elpida
{
	/// Forces the compiler to treat 'value' as read and modified, so computations
	/// producing it cannot be removed or constant-folded. It emits no instructions
	/// apart from keeping 'value' in memory at this point.
	template<typename T>
	inline void DoNotOptimize(T& value)
	{
		asm volatile("" : "+m"(value) : : "memory");
	}
}

#endif //ELPIDA_OPTIMIZATION_HPP
