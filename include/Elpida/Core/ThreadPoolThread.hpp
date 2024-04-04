//
// Created by klapeto on 2/4/2024.
//

#ifndef ELPIDA_THREADPOOLTHREAD_HPP
#define ELPIDA_THREADPOOLTHREAD_HPP

#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Core/BlockingCollection.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

namespace Elpida
{

	class ThreadPoolThread final
	{
	public:
		class Function
		{
		public:
			virtual void operator()() = 0;
		};

		void Excecute(std::unique_ptr<Function>&& function)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_function = std::move(function);
			_execute = true;
			_notifier.notify_all();
		}

		void Terminate()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_keepGoing = false;
			_function.reset();
			_execute = true;
			_notifier.notify_all();
			lock.unlock();

			_thread.join();
		}

		explicit ThreadPoolThread(const Ref<const ProcessingUnitNode>& targetProcessor)
				: _targetProcessor(targetProcessor), _execute(false), _keepGoing(true)
		{
			_thread = std::thread(&ThreadPoolThread::Procedure, this);
		}

		~ThreadPoolThread()
		{
			Terminate();
		}

	private:
		std::thread _thread;
		std::mutex _mutex;
		std::condition_variable _notifier;
		const Ref<const ProcessingUnitNode>& _targetProcessor;
		std::unique_ptr<Function> _function;
		bool _execute;
		bool _keepGoing;

		void Procedure()
		{
			_targetProcessor.get().PinThreadToThisProcessor();
			while (_keepGoing)
			{
				std::unique_lock<std::mutex> lock(_mutex);
				while (!_execute)
				{
					_notifier.wait(lock);
				}
				_execute = false;
				auto function = std::move(_function);
				lock.unlock();
				if (function)
				{
					(*function)();
				}
			}
		}
	};

} // Elpida

#endif //ELPIDA_THREADPOOLTHREAD_HPP
