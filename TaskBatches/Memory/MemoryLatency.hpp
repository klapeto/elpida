/*
 * MemoryLatency.hpp
 *
 *  Created on: 19 Μαΐ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_MEMORYLATENCY_HPP_
#define TASKBATCHES_MEMORY_MEMORYLATENCY_HPP_

#include <Elpida/Task.hpp>
#include <Elpida/Types/Integer.hpp>
#include <Elpida/Types/Primitives.hpp>
#include <Elpida/TaskRunResult.hpp>
#include <Elpida/TaskMetrics.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <TaskBatches/General/Memory.hpp>

#include <chrono>
#include <Elpida/Timer.hpp>

namespace Elpida
{

	template<typename T = int64_t>
	class MemoryLatency: public Task
	{
		public:
			virtual void run() override
			{
				volatile auto ptr = (T*) _memory.getPointer();
				register auto start = ptr;
				register auto end = (T*) ((T) start + _memory.getSize());
				register auto itterations = _itterations;
				register auto x = T();
				for (register auto i = 0ul; i < itterations; ++i)
				{
					ptr = start;
					while (ptr < end)
					{
						x = *ptr;
						x = *(ptr + 1);
						x = *(ptr + 2);
						x = *(ptr + 3);
						x = *(ptr + 4);
						x = *(ptr + 5);
						x = *(ptr + 6);
						x = *(ptr + 7);
						x = *(ptr + 8);
						x = *(ptr + 9);
						x = *(ptr + 10);
						x = *(ptr + 11);
						x = *(ptr + 12);
						x = *(ptr + 13);
						x = *(ptr + 14);
						x = *(ptr + 15);
						x = *(ptr + 16);
						x = *(ptr + 17);
						x = *(ptr + 18);
						x = *(ptr + 19);
						x = *(ptr + 20);
						x = *(ptr + 21);
						x = *(ptr + 22);
						x = *(ptr + 23);
						x = *(ptr + 24);
						x = *(ptr + 25);
						x = *(ptr + 26);
						x = *(ptr + 27);
						x = *(ptr + 28);
						x = *(ptr + 29);
						x = *(ptr + 30);
						x = *(ptr + 31);
						ptr += 32;
					}
				}
				auto dummy = x;
			}

			unsigned long getItterations() const
			{
				return _itterations;
			}

			void calculateResults(const TaskMetrics& metrics) override
			{
				auto size = _itterations * _memory.getSize();
				auto time = metrics.getSubdivision<TaskMetrics::NanoSecond>();
				_runResult.setOriginalValue((Float64) time / (Float64) size);
				_runResult.setCustom(true);
				_runResult.setMultiplier(1000);
				_runResult.setTestedDataValue(_memory.getSize());
				_runResult.setValueName("ns");
				addResult(_runResult);
			}

			MemoryLatency(const Memory& memory)
					: Task("Memory Read latency: " + ValueUtilities::getValueScaleString(memory.getSize()) + "B"), _memory(memory)
			{
				_itterations = _itterationConstant / (Float64) _memory.getSize();
			}

			virtual ~MemoryLatency()
			{
				finalize();
			}

		private:
			TaskRunResult _runResult;
		protected:
			const Memory& _memory;
			unsigned long _itterations;
		private:
			static constexpr Float64 _itterationConstant = 100000000000; // rough estimate, to be passed on construction later once we find the latency
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_MEMORYLATENCY_HPP_ */
