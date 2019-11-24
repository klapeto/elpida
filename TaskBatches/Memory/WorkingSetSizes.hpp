/*
 * WorkingSetSizes.hpp
 *
 *  Created on: 24 Νοε 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_WORKINGSETSIZES_HPP_
#define TASKBATCHES_MEMORY_WORKINGSETSIZES_HPP_

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

			WorkingSetSizes() = delete;
			~WorkingSetSizes() = delete;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_WORKINGSETSIZES_HPP_ */
