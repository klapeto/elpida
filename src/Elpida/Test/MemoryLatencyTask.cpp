//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Test/MemoryLatencyTask.hpp"

#include "Elpida/TaskData.hpp"

#include <cstdlib>
#include <vector>
#include <random>
#include <algorithm>

#define REPEAT_5(x) x;x;x;x
#define REPEAT_10(x) REPEAT_5(x);REPEAT_5(x)
#define REPEAT_50(x) REPEAT_10(x);REPEAT_10(x);REPEAT_10(x);REPEAT_10(x);REPEAT_10(x)
#define REPEAT_100(x) REPEAT_50(x);REPEAT_50(x)

namespace Elpida
{
	static std::size_t calculateNumberOfBitsOfNumber(std::size_t number)
	{
		std::size_t bits = 0;
		for (std::size_t i = number >> 1; i != 0; i >>= 1, bits++);
		return bits;
	}

	static std::vector<size_t> calculateLines(std::size_t linesCount, std::size_t lineSize)
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

		std::vector<std::size_t> returnLines(linesCount, 0);

		std::size_t bits = calculateNumberOfBitsOfNumber(linesCount);

		for (std::size_t i = 0; i < linesCount; ++i)
		{
			for (std::size_t j = 0; j < bits; j++)
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

	static std::vector<std::size_t> calculatePages(std::size_t pagesCount, std::size_t pageSize)
	{
		std::random_device rd;
		std::mt19937_64 generator(rd());

		std::vector<std::size_t> returnPages(pagesCount, 0);

		// Generate the indexes for each page
		for (std::size_t i = 0; i < pagesCount; ++i)
		{
			returnPages[i] = i * pageSize;
		}

		// Randomize the page collection
		std::shuffle(returnPages.begin(), returnPages.end(), generator);

		return returnPages;
	}

	static void assignPointers(char* basePtr,
		const std::vector<std::size_t>& lines,
		std::size_t pageA,
		std::size_t pageB,
		std::size_t lineIndexOffsetA,
		std::size_t lineIndexOffsetB)
	{
		const auto linesCount = lines.size();
		for (std::size_t i = 0; i < linesCount; ++i)
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

	void MemoryLatencyTask::Prepare(TaskData&& data)
	{
		// modified/modernized version of lmbech thrashing/latency algorithm
		// Check the paper here:
		// https://www.usenix.org/legacy/publications/library/proceedings/sd96/full_papers/mcvoy.pdf

		_data = std::move(data);
		_data.Allocate(_size);

		const std::size_t linesPerPage = std::max((double)_pageSize / (double)_cacheLineSize, 1.0);
		const std::size_t pagesCount = std::max((double)_size / (double)_pageSize, 1.0);

		auto pages = calculatePages(pagesCount, _pageSize);
		_ptr = (char*)_data.GetDataRaw();

		if (pagesCount > 1)
		{
			auto lines = calculateLines(linesPerPage, _cacheLineSize);

			const size_t lastPageIndex = pagesCount - 1;

			for (std::size_t i = 0; i < lastPageIndex; ++i)
			{
				// pageIndexA will be a random page
				auto pageIndexA = pages[i];

				// pageIndexB will also be a random page
				auto pageIndexB = pages[i + 1];

				assignPointers(_ptr, lines, pageIndexA, pageIndexB, i, i + 1);
			}

			// For the last random page we should 'reference' it
			// to the first page. Keep in mind this is not the
			// last page in the continuous memory but the last
			// page we processed above which is a random page.
			auto lastPage = pages[lastPageIndex];
			auto firstPage = pages[0];

			assignPointers(_ptr, lines, lastPage, firstPage, lastPageIndex, 1);
		}
		else
		{
			auto lines = calculateLines(_size / _cacheLineSize, _cacheLineSize);
			// Too small size, fits in a single page
			auto pageIndexA = pages[0];
			auto pageIndexB = pages[0];

			assignPointers(_ptr, lines, pageIndexA, pageIndexB, 0, 1);
		}

	}

	MemoryLatencyTask::MemoryLatencyTask(std::size_t size, std::size_t cacheLineSize, std::size_t pageSize)
		: MicroTask(), _ptr(nullptr), _size(size), _cacheLineSize(cacheLineSize), _pageSize(pageSize)
	{

	}
	TaskData MemoryLatencyTask::Finalize()
	{
		return std::move(_data);
	}

	TaskInfo MemoryLatencyTask::GetInfo() const
	{
		return {
			"Memory latency",
			"Calculates the average access latency of memory.",
			"",
			"The average access time.",
			ScoreType::Time,
			Elpida::DataInfo(),
			Elpida::DataInfo()
		};
	}

	void MemoryLatencyTask::DoRun(std::size_t iterations)
	{
		volatile auto* ptr = (char**)_ptr;

		while (iterations-- > 0)
		{
			REPEAT_100(ptr = (char**)*ptr);
			REPEAT_100(ptr = (char**)*ptr);
			REPEAT_100(ptr = (char**)*ptr);
			REPEAT_100(ptr = (char**)*ptr);
			REPEAT_100(ptr = (char**)*ptr);
			REPEAT_100(ptr = (char**)*ptr);
			REPEAT_100(ptr = (char**)*ptr);
			REPEAT_100(ptr = (char**)*ptr);
			REPEAT_100(ptr = (char**)*ptr);
			REPEAT_100(ptr = (char**)*ptr);
		}
	}

	std::size_t MemoryLatencyTask::GetOperationsPerformedPerRun()
	{
		return 1000;
	}

	Duration MemoryLatencyTask::GetExecutionMinimumDuration()
	{
		return Seconds(10);
	}

	bool MemoryLatencyTask::CanBeMultiThreaded() const
	{
		return false;
	}

	std::unique_ptr<Task> MemoryLatencyTask::DoDuplicate() const
	{
		return std::unique_ptr<Task>(new MemoryLatencyTask(_size, _cacheLineSize, _pageSize));
	}

} // Elpida