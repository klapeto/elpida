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


#ifndef TASKBATCHES_MEMORY_WORKINGSETSIZES_HPP_
#define TASKBATCHES_MEMORY_WORKINGSETSIZES_HPP_

#include <cstdint>
#include <string>
#include <tuple>

namespace Elpida
{
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

		static inline const std::tuple<const std::string, const int> BenchmarkValues[] = {

                {"5e8d84fe-9925-4798-84e0-573fcb2e305c",B_512},

                {"9518c817-c566-4bac-bf95-7c18d88454c2",KiB_2},
                {"bcba5680-b088-43b7-8182-1216a8d13564",KiB_4},
                {"83fa1ecb-23a7-4d73-9e81-f804624fbb34",KiB_8},
                {"3acd97fa-480a-49e1-aca8-599bc0e98727",KiB_16},
                {"abbe14cf-1046-4845-9f8c-4de4b0f75e7a",KiB_32},
                {"12109f85-0f20-49eb-b123-9f1ee0e9c475",KiB_64},
                {"14d63060-c8ea-4d8c-94ae-032d0eb3bed7",KiB_128},
                {"f5c98fba-44f8-43ea-b080-6386c5cf1c4a",KiB_256},
                {"5939c23a-f2a9-46f8-a6a7-e180f7697187",KiB_512},

                {"2e6e09e5-7767-4caf-9756-3dfc6ec7479b",MiB_2},
                {"fa92a536-3649-42cf-adfa-9b429ae0520d",MiB_4},
                {"dd6765b5-a467-4ca4-afb4-9157a8d7b5ff",MiB_8},
                {"193ae1da-1a83-4a2b-af02-557e91db78d9",MiB_16},
                {"0b76c53a-c169-4950-ba00-811897e8df6c",MiB_32},
                {"2af7b5b7-7da6-424f-90f5-7520b01ecb78",MiB_64},
                {"f9c5e17d-e58c-4811-b465-5cce62b74e01",MiB_128},
                {"1c1e2b44-65b4-4fac-b023-31f269a72001",MiB_256},
                {"b9f3341e-2f67-4eb9-ac9c-57a7fc31caaf",MiB_512},
		};

		WorkingSetSizes() = delete;
		~WorkingSetSizes() = delete;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_WORKINGSETSIZES_HPP_ */
