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

/*
 * WorkingSetSizes.hpp
 *
 *  Created on: 24 Νοε 2019
 *      Author: klapeto
 */

#include <cstdint>

#ifndef TASKBATCHES_MEMORY_WORKINGSETSIZES_HPP_
#define TASKBATCHES_MEMORY_WORKINGSETSIZES_HPP_

namespace Elpida
{

	typedef
#if __x86_64__ || _M_AMD64
	int64_t
#elif __i386__ || __i386 || _X86_
	int32_t
#else
#error Elipda currently only supports X86-64 and X86 achritectures
#endif
		RegisterSize;

	class WorkingSetSizes
	{
	public:
		enum
		{
			B_4 = 4,
			B_8 = 8,
			B_16 = 16,
			B_32 = 32,
			B_64 = 64,
			B_128 = 128,
			B_256 = 256,
			B_512 = 512,
			KiB_1 = 1024,
			KiB_2 = KiB_1 * 2,
			KiB_4 = KiB_1 * 4,
			KiB_8 = KiB_1 * 8,
			KiB_16 = KiB_1 * 16,
			KiB_32 = KiB_1 * 32,
			KiB_64 = KiB_1 * 64,
			KiB_128 = KiB_1 * 128,
			KiB_256 = KiB_1 * 256,
			KiB_512 = KiB_1 * 512,
			MiB_1 = KiB_1 * 1024,
			MiB_2 = MiB_1 * 2,
			MiB_4 = MiB_1 * 4,
			MiB_8 = MiB_1 * 8,
			MiB_16 = MiB_1 * 16,
			MiB_32 = MiB_1 * 32,
			MiB_64 = MiB_1 * 64,
			MiB_128 = MiB_1 * 128,
			MiB_256 = MiB_1 * 256,
			MiB_512 = MiB_1 * 512,
		};

		static constexpr inline int Values[] = {
			B_256, B_512,
			KiB_1, KiB_2, KiB_4, KiB_8, KiB_16, KiB_32, KiB_64, KiB_128, KiB_256, KiB_512,
			MiB_1, MiB_2, MiB_4, MiB_8, MiB_16, MiB_32, MiB_64, MiB_128, MiB_256, MiB_512,
		};

		WorkingSetSizes() = delete;
		~WorkingSetSizes() = delete;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_WORKINGSETSIZES_HPP_ */
