//
// Created by klapeto on 20/4/20.
//

#include <cmath>
#include "Elpida/Engine/Task/MultiThreadTask.hpp"
#include "Elpida/Engine/Benchmark.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskData.hpp"

namespace Elpida
{

	MultiThreadTask::MultiThreadTask(const TaskSpecification& taskSpecification,
		const TaskConfiguration& configuration,
		const TaskAffinity& affinity)
		: Task(taskSpecification, affinity, true), _configuration(configuration)
	{
	}

	void MultiThreadTask::prepare()
	{
		const auto& processors = _affinity.getProcessorNodes();
		const size_t processorCount = processors.size();

		if (_specification.acceptsInput())
		{
			breakInputIntoChunks(processorCount);
		}
		size_t i = 0;
		for (auto processor : processors)
		{
			auto task = _specification
				.createNewTask(_configuration, TaskAffinity(std::vector<const ProcessorNode*>{ processor }));

			if (_specification.acceptsInput())
			{
				task->setInput(&_chunks[i]);
			}

			task->prepare();
			auto thread = TaskThread(*task, _wakeNotifier, _mutex, _threadsShouldWake, processor->getOsIndex());
			thread.start();
			_createdThreads.push_back(std::move(thread));
		}
	}

	void MultiThreadTask::finalize()
	{
		for (auto& thread: _createdThreads)
		{
			thread.getTaskToRun().finalize();
		}
		_createdThreads.clear();
		destroyChunks();
		if (_specification.exportsOutput())
		{
			_outputData = *_inputData;
		}
	}
	void MultiThreadTask::run()
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
		size_t chunkSize = 0;
		auto dataSize = (*_inputData)->getSize();

		if (_inputData != nullptr && dataSize > 0)
		{
			chunkSize = std::floor(dataSize / (float)chunks);
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME,
				"A task needs input but the previous one does not provide output!");
		}

		if (dataSize < chunks)
		{
			throw ElpidaException(FUNCTION_NAME,
				"A task received data as input that is less than the chunks");
		}

		if (dataSize < chunkSize)    //is this possible?
		{
			throw ElpidaException(FUNCTION_NAME,
				"A task received data as input that is less than the chunk size");
		}

		auto rootPtr = (*_inputData)->getData();
		size_t offset = 0;
		for (size_t i = 0; i < chunks; ++i)
		{
			if (offset + chunkSize <= dataSize)
			{
				_chunks.push_back(new TaskData(rootPtr + offset, chunkSize));
				offset += chunkSize;
			}
			else
			{
				auto newChunkSize = dataSize - offset + chunkSize;
				_chunks.push_back(new TaskData(rootPtr + offset, newChunkSize));
			}
		}
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
}