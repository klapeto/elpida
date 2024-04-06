//
// Created by klapeto on 2/4/2024.
//

#ifndef ELPIDA_THREADPOOLTHREAD_HPP
#define ELPIDA_THREADPOOLTHREAD_HPP

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

		explicit ThreadPoolThread();
		~ThreadPoolThread();
	private:
		std::thread _thread;
		std::mutex _mutex;
		std::condition_variable _notifier;
		std::unique_ptr<Functor> _function;
		bool _execute;
		bool _keepGoing;

		void Procedure();
	};

} // Elpida

#endif //ELPIDA_THREADPOOLTHREAD_HPP
