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
 * MemoryLatench.cpp
 *
 *  Created on: 19 Μαΐ 2019
 *      Author: klapeto
 */

#include "Benchmarks/Memory/Latency/MemoryLatency.hpp"

#include "Benchmarks/Memory/WorkingSetSizes.hpp"

#include <Elpida/ElpidaException.hpp>
#include <Elpida/Utilities/Duration.hpp>
#include <Elpida/SystemInfo/MemoryInfo.hpp>
#include <Elpida/SystemInfo/CpuInfo.hpp>
#include <Elpida/ServiceProvider.hpp>
#include <Elpida/Macros.hpp>

namespace Elpida
{

	MemoryLatency::MemoryLatency(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t memorySize,
		size_t iterationsToRun)
		: MicroTask(specification, processorToRun, serviceProvider, iterationsToRun),
		  _memory(memorySize, processorToRun)
	{

	}

	static size_t calculateNumberOfBitsOfNumber(size_t number)
	{
		size_t bits = 0;
		for (size_t i = number >> 1; i != 0; i >>= 1, bits++);
		return bits;
	}

	static std::vector<size_t> calculateLines(size_t linesCount, size_t lineSize)
	{
		/*	From original lmbench 'words_initialize'
		 *
		 *  This is supposed to create the order in which the words in a
		 *  "cache line" are used.  Since we rarely know the cache line
		 *  size with any real reliability, we need to jump around so
		 *  as to maximize the number of potential cache misses, and to
		 *  minimize the possibility of re-using a cache line.
		 *  */

		// From my understanding, the way it achieves the above is
		// by creating values that are 'mirrored' from their index.
		// For instance the value in index = 3 and word size 8
		// 		i => 0000 0011
		// becomes 'mirrored' in bits like
		//		value => 1100 0000
		// which in the end we have
		//		returnLines[3] = (binary) 1100 0000 = 192

		std::vector<size_t> returnLines(linesCount, 0);

		size_t bits = calculateNumberOfBitsOfNumber(linesCount);

		for (size_t i = 0; i < linesCount; ++i)
		{
			for (size_t j = 0; j < bits; j++)
			{
				// j = bit index
				// search for bits in this i

				// check if this bit index has 1 in the i
				if (i & (1 << j))
				{
					// Reverse the bit and 'append' it to the existing
					// end value

					// j = bit index
					// bits = max bits = highest bit
					returnLines[i] |= (1 << (bits - j - 1));
				}
			}

			// multiply the end value with the lineSize to
			// get a meaningful value to use
			returnLines[i] *= lineSize;
		}

		return returnLines;
	}

	static std::vector<size_t> calculatePages(size_t pagesCount, size_t pageSize)
	{
		std::random_device rd;
		std::mt19937_64 generator(rd());

		std::vector<size_t> returnPages(pagesCount, 0);

		// Generate the indexes for each page
		for (size_t i = 0; i < pagesCount; ++i)
		{
			returnPages[i] = i * pageSize;
		}

		// Randomize the page collection
		std::shuffle(returnPages.begin(), returnPages.end(), generator);

		return returnPages;
	}

	static size_t getCacheLineSize(const CpuInfo& cpuInfo)
	{
		constexpr size_t fallbackCacheLine = 64;

		auto& caches = cpuInfo.getCaches();
		if (!caches.empty()) return caches.front().getLineSize();

		return fallbackCacheLine;
	}

	static void assignPointers(char* basePtr,
		const std::vector<size_t>& lines,
		size_t pageA,
		size_t pageB,
		size_t lineIndexOffsetA,
		size_t lineIndexOffsetB)
	{
		const auto linesCount = lines.size();
		for (size_t i = 0; i < linesCount; ++i)
		{
			// Indices are calculated by the respective page indices and a 'random'
			// line. This 'random' line shifts +1 for each page iteration
			auto indexA = pageA + lines[(lineIndexOffsetA + i) % linesCount];
			auto indexB = pageB + lines[(lineIndexOffsetB + i) % linesCount];

			// Assign the target pointer to the current.
			// This means that when we dereference t
			*(char**)&basePtr[indexA] = &basePtr[indexB];
		}
	}

	void MemoryLatency::prepareImpl()
	{
		// modified/modernized version of lmbech thrashing/latency algorithm
		// Check the paper here:
		// https://www.usenix.org/legacy/publications/library/proceedings/sd96/full_papers/mcvoy.pdf

		_memory.allocate();

		const size_t size = _memory.getSize();
		const size_t lineSize = getCacheLineSize(_serviceProvider.getCpuInfo());
		const size_t pageSize = _serviceProvider.getMemoryInfo().getPageSize();
		const size_t linesPerPage = std::max(pageSize / (double)lineSize, 1.0);
		const size_t pagesCount = std::max(size / (double)pageSize, 1.0);

		auto pages = calculatePages(pagesCount, pageSize);
		auto basePtr = (char*)_memory.getPointer();

		if (pagesCount > 1)
		{
			auto lines = calculateLines(linesPerPage, lineSize);

			const size_t lastPageIndex = pagesCount - 1;

			for (std::size_t i = 0; i < lastPageIndex; ++i)
			{
				// pageIndexA will be a random page
				auto pageIndexA = pages[i];

				// pageIndexB will also be a random page
				auto pageIndexB = pages[i + 1];

				assignPointers(basePtr, lines, pageIndexA, pageIndexB, i, i + 1);
			}

			// For the last random page we should 'reference' it
			// to the first page. Keep in mind this is not the
			// last page in the continuous memory but the last
			// page we processed above which is a random page.
			auto lastPage = pages[lastPageIndex];
			auto firstPage = pages[0];

			assignPointers(basePtr, lines, lastPage, firstPage, lastPageIndex, 1);
		}
		else
		{
			auto lines = calculateLines(size / lineSize, lineSize);
			// Too small size, fits in a single page
			auto pageIndexA = pages[0];
			auto pageIndexB = pages[0];

			assignPointers(basePtr, lines, pageIndexA, pageIndexB, 0, 1);
		}
	}

	void MemoryLatency::run(size_t iterations)
	{
		volatile auto* ptr = (char**)_memory.getPointer();

		while (iterations-- > 0)
		{
			REPEAT_100(ptr = (char**)*ptr);
		}
	}

	double MemoryLatency::getInputDataSize() const
	{
		return _memory.getSize();
	}

	double MemoryLatency::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return taskElapsedTime.count() / Duration::period::den / 100.0;
	}

	size_t MemoryLatency::getMinimumIterations() const
	{
		return std::max(_memory.getSize() / (getCacheLineSize(_serviceProvider.getCpuInfo()) * 100) + 1,
			MinimumIterations);
	}

} /* namespace Elpida */
