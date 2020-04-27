//
// Created by klapeto on 20/4/20.
//

#include <cmath>
#include "Elpida/Engine/Task/MultiThreadTask.hpp"
#include "Elpida/Engine/Benchmark/Benchmark.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/Data/TaskData.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{

	MultiThreadTask::MultiThreadTask(const TaskSpecification& taskSpecification,
		const TaskConfiguration& configuration,
		const TaskAffinity& affinity)
		: Task(taskSpecification, affinity, true), _configuration(configuration), _threadsShouldWake(false)
	{
	}

	void MultiThreadTask::execute()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_threadsShouldWake = true;
		lock.unlock();
		_wakeNotifier.notify_all();
		for (auto& thread : _createdThreads)
		{
			thread.join();
		}
	}

	void MultiThreadTask::applyAffinity()
	{

	}

	void MultiThreadTask::breakInputIntoChunks(size_t chunks)
	{
		destroyChunks();

//		size_t chunkSize = 0;
//		auto inputData = getInput();
//		bool acceptsInput = _specification.acceptsInput();
//
//		auto dataSize = inputData.getTaskData().size();
//
//		if (acceptsInput && dataSize > 0)
//		{
//			chunkSize = std::floor(dataSize / (float)chunks);
//		}
//		else
//		{
//			throw ElpidaException(FUNCTION_NAME,
//				"A task needs input but the previous one does not provide output!");
//		}
//
//		if (dataSize < chunks)
//		{
//			throw ElpidaException(FUNCTION_NAME,
//				"A task received data as input that is less than the chunks");
//		}
//
//		if (dataSize < chunkSize)    //is this possible?
//		{
//			throw ElpidaException(FUNCTION_NAME,
//				"A task received data as input that is less than the chunk size");
//		}
//
//		auto rootPtr = inputData.getTaskData().begin();
//		size_t offset = 0;
//		for (size_t i = 0; i < chunks; ++i)
//		{
//			if (offset + chunkSize <= dataSize)
//			{
//				_chunks.push_back(new TaskData(rootPtr + offset, chunkSize));
//				offset += chunkSize;
//			}
//			else
//			{
//				auto newChunkSize = dataSize - offset + chunkSize;
//				_chunks.push_back(new TaskData(rootPtr + offset, newChunkSize));
//			}
//		}
	}

	void MultiThreadTask::destroyChunks()
	{
		for (auto chunk: _chunks)
		{
			delete chunk;
		}
		_chunks.clear();
	}

	MultiThreadTask::~MultiThreadTask()
	{
		destroyChunks();
	}

	void MultiThreadTask::prepareImpl()
	{
		const auto& processors = _affinity.getProcessorNodes();
		const size_t processorCount = processors.size();

		if (_specification.acceptsInput())
		{

		}

//
//		if (_specification.acceptsInput())
//		{
//			breakInputIntoChunks(processorCount);
//		}
//		size_t i = 0;
//		for (auto processor : processors)
//		{
//			auto task = _specification
//				.createNewTask(_configuration, TaskAffinity(std::vector<const ProcessorNode*>{ processor }));
//
//			if (_specification.acceptsInput())
//			{
//				task->setInput(*_chunks[i]);
//			}
//
//			task->prepare();
//			_createdThreads.push_back(std::move(TaskThread(*task,
//				_wakeNotifier,
//				_mutex,
//				_threadsShouldWake,
//				processor->getOsIndex())));
//			_createdThreads.back().start();
//		}
	}

	TaskOutput MultiThreadTask::finalizeAndGetOutputData()
	{
		for (auto& thread: _createdThreads)
		{
			thread.getTaskToRun().finalize();
		}
		_createdThreads.clear();
		destroyChunks();
//		if (_specification.exportsOutput() && _specification.acceptsInput())
//		{
//			_outputData = *_inputData;
//		}
//		else if (_specification.exportsOutput())
//		{
//			// ???
//		}
		return TaskOutput();
	}
}