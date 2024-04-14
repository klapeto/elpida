//
// Created by klapeto on 2/4/2024.
//

#ifndef ELPIDA_THREADPOOLTHREAD_HPP
#define ELPIDA_THREADPOOLTHREAD_HPP

#include "Elpida/Core/ThreadPool/Functor.hpp"
#include "Elpida/Core/BlockingCollection.hpp"

#include <thread>
#include <mutex>
#include <atomic>
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
		BlockingCollection<std::unique_ptr<Functor>> _queue;
		std::thread _thread;
		std::atomic<bool> _keepGoing;

		void Procedure();
	};

} // Elpida

#endif //ELPIDA_THREADPOOLTHREAD_HPP
