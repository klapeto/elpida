//
// Created by klapeto on 2/4/2024.
//

#ifndef ELPIDA_THREADPOOLTHREAD_HPP
#define ELPIDA_THREADPOOLTHREAD_HPP

#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Core/ThreadPool/Functor.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>

namespace Elpida
{

	class ThreadPoolThread final
	{
	public:

		void Excecute(std::unique_ptr<Functor>&& function);
		void Terminate();

		explicit ThreadPoolThread(const Ref<const ProcessingUnitNode>& targetProcessor, bool pin);
		~ThreadPoolThread();
	private:
		std::thread _thread;
		std::mutex _mutex;
		std::condition_variable _notifier;
		std::unique_ptr<Functor> _function;
		const Ref<const ProcessingUnitNode>& _targetProcessor;
		bool _execute;
		bool _keepGoing;
		bool _pin;

		void Procedure();
	};

} // Elpida

#endif //ELPIDA_THREADPOOLTHREAD_HPP
