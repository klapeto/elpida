//
// Created by klapeto on 27/2/2023.
//

#ifndef _MEMORYLATENCYTASK_HPP_
#define _MEMORYLATENCYTASK_HPP_

#include <cstdlib>

#include "Elpida/MicroTask.hpp"

namespace Elpida
{

	class MemoryLatencyTask : public MicroTask
	{
	 public:
		void Prepare(TaskData& inputData) override;
		void Finalize(TaskData& outputData) override;
		TaskInfo GetInfo() const override;
		MemoryLatencyTask(std::size_t size, std::size_t cacheLineSize, std::size_t pageSize);
		~MemoryLatencyTask() override = default;
	 protected:
		void DoRun(std::size_t iterations) override;
		size_t GetOperationsPerformedPerRun() override;
	 private:
		char* _data;
		std::size_t _size;
		std::size_t _cacheLineSize;
		std::size_t _pageSize;
	};

} // Elpida

#endif //_MEMORYLATENCYTASK_HPP_
