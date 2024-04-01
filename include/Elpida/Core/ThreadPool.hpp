//
// Created by klapeto on 31/3/2024.
//

#ifndef ELPIDA_THREADPOOL_HPP
#define ELPIDA_THREADPOOL_HPP

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/BlockingCollection.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"

#include <future>
#include <mutex>
#include <functional>

namespace Elpida
{

	class ThreadPool final
	{
	private:
		class Delegate
		{
		public:
			virtual void Run() = 0;
		};

		template<typename T, typename TCallable>
		class PromiseDelegate : public Delegate
		{
		public:
			void Run() override
			{
				try
				{
					_promise.set_value(_callable());
				}
				catch (...)
				{
					_promise.set_exception(std::exception_ptr());
				}
			}

			PromiseDelegate(TCallable callable, std::promise<T>&& promise)
					: _callable(callable), _promise(std::move(promise))
			{

			}

		private:
			TCallable _callable;
			std::promise<T> _promise;
		};

	public:

		template<typename T, typename TCallable>
		std::future<T> Queue(TCallable callable)
		{
			std::promise<T> promise;
			auto future = promise.get_future();

			auto id = GetNextId();

			auto& collection = _collections[id];

			collection->Add(new PromiseDelegate(callable, std::move(promise)));

			return std::move(future);
		}

		explicit ThreadPool(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors)
				: _targetProcessors(targetProcessors), _nextProcessor(0), _keepGoing(true)
		{
			for (std::size_t i = 0; i < targetProcessors.size(); ++i)
			{
				_collections.push_back(std::make_unique<BlockingCollection<Delegate*>>());
			}

			for (std::size_t i = 0; i < targetProcessors.size(); ++i)
			{
				_threads.emplace_back(&ThreadPool::Procedure, this, targetProcessors[i], _collections[i].get());
			}
		}

		~ThreadPool()
		{
			_keepGoing = false;

			for (auto& collection: _collections)
			{
				collection->Destroy();
			}

			for (auto& th: _threads)
			{
				th.join();
			}
		}

	private:

		std::size_t GetNextId()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (++_nextProcessor >= _targetProcessors.size())
			{
				_nextProcessor = 0;
			}
			return _nextProcessor;
		}

		std::mutex _mutex;
		Vector<Ref<const ProcessingUnitNode>> _targetProcessors;
		Vector<std::thread> _threads;
		Vector<UniquePtr<BlockingCollection<Delegate*>>> _collections;
		std::size_t _nextProcessor;
		bool _keepGoing;

		void Procedure(const Ref<const ProcessingUnitNode>& processingUnitNode,
				BlockingCollection<Delegate*>* collection) const
		{
			processingUnitNode.get().PinThreadToThisProcessor();
			while (_keepGoing)
			{
				auto value = collection->Get();
				if (value != nullptr)
				{
					value->Run();
				}
			}
		}
	};

} // Elpida

#endif //ELPIDA_THREADPOOL_HPP
