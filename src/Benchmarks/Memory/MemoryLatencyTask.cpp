//
// Created by klapeto on 27/2/2023.
//

#include "MemoryLatencyTask.hpp"

#include "Elpida/Core/RawTaskData.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Repeat.hpp"

#include <random>
#include <algorithm>

namespace Elpida
{
	static Size calculateNumberOfBitsOfNumber(Size number)
	{
		Size bits = 0;
		for (Size i = number >> 1; i != 0; i >>= 1, bits++);
		return bits;
	}

	static Vector<Size> calculateLines(Size linesCount, Size lineSize)
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

		Vector<Size> returnLines(linesCount, 0);

		Size bits = calculateNumberOfBitsOfNumber(linesCount);

		for (Size i = 0; i < linesCount; ++i)
		{
			for (Size j = 0; j < bits; j++)
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

	static Vector<Size> calculatePages(Size pagesCount, Size pageSize)
	{
		std::random_device rd;
		std::mt19937_64 generator(rd());

		Vector<Size> returnPages(pagesCount, 0);

		// Generate the indexes for each page
		for (Size i = 0; i < pagesCount; ++i)
		{
			returnPages[i] = i * pageSize;
		}

		// Randomize the page collection
		std::shuffle(returnPages.begin(), returnPages.end(), generator);

		return returnPages;
	}

	static void assignPointers(char* basePtr,
		const Vector<Size>& lines,
		Size pageA,
		Size pageB,
		Size lineIndexOffsetA,
		Size lineIndexOffsetB)
	{
		const auto linesCount = lines.size();
		for (Size i = 0; i < linesCount; ++i)
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

	void MemoryLatencyTask::Prepare(SharedPtr<AbstractTaskData> data)
	{
		// modified/modernized version of lmbech thrashing/latency algorithm
		// Check the paper here:
		// https://www.usenix.org/legacy/publications/library/proceedings/sd96/full_papers/mcvoy.pdf

		_data = std::move(data);
		_data->Allocate(_size);

		const Size linesPerPage = std::max((double)_pageSize / (double)_cacheLineSize, 1.0);
		const Size pagesCount = std::max((double)_size / (double)_pageSize, 1.0);

		auto pages = calculatePages(pagesCount, _pageSize);
		_ptr = (char*)_data->GetData();

		if (pagesCount > 1)
		{
			auto lines = calculateLines(linesPerPage, _cacheLineSize);

			const size_t lastPageIndex = pagesCount - 1;

			for (Size i = 0; i < lastPageIndex; ++i)
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

	MemoryLatencyTask::MemoryLatencyTask(Size size, Size cacheLineSize, Size pageSize)
		: MicroTask(), _ptr(nullptr), _size(size), _cacheLineSize(cacheLineSize), _pageSize(pageSize)
	{

	}

	SharedPtr<AbstractTaskData> MemoryLatencyTask::Finalize()
	{
		return std::move(_data);
	}

	TaskInfo MemoryLatencyTask::DoGetInfo() const
	{
		return {
			"Memory latency",
			"Calculates the average access latency of memory.",
			"",
			"The average access time.",
			ScoreType::Time
		};
	}

	void MemoryLatencyTask::DoRun(Iterations iterations)
	{
		volatile auto* ptr = (char**)_ptr;

		while (iterations-- > 0)
		{
			REPEAT_1000(ptr = (char**)*ptr);
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

	UniquePtr<Task> MemoryLatencyTask::DoDuplicate() const
	{
		return UniquePtr<Task>(new MemoryLatencyTask(_size, _cacheLineSize, _pageSize));
	}

	Size MemoryLatencyTask::GetProcessedDataSize() const
	{
		return _size;
	}

} // Elpida