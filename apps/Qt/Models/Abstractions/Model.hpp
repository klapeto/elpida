//
// Created by klapeto on 13/4/20.
//

#ifndef APPS_QT_CORE_ABSTRACTIONS_MODEL_HPP
#define APPS_QT_CORE_ABSTRACTIONS_MODEL_HPP

#include <Elpida/Event.hpp>
#include <Elpida/ElpidaException.hpp>

namespace Elpida
{

	class Model
	{
	public:
		Event<> dataChanged;

		template<typename T, typename TCallable>
		void transactional(TCallable callable)
		{
			static_assert(std::is_base_of_v<Model, T>, "Only derived classes can be used for template parameter");
			try
			{
				beginUpdateTransaction();
				callable((T&)*this);
				endUpdateTransaction();
			}
			catch (...)
			{
				if (_lock.owns_lock())
				{
					_lock.unlock();
				}
				_transaction = false;
				throw;
			}
		}

		Model()
			: _lock(_mutex, std::defer_lock_t()), _transaction(false)
		{

		}

		virtual ~Model() = default;
	protected:
		std::mutex _mutex;
		std::unique_lock<std::mutex> _lock;

		bool _transaction;
		void onDataChanged()
		{
			if (!_transaction && !_lock.owns_lock())
			{
				dataChanged.raise();
			}
		}

		void beginUpdateTransaction()
		{
			_lock.lock();
			_transaction = true;
		}

		void endUpdateTransaction()
		{
			if (_lock.owns_lock())
			{
				_transaction = false;
				_lock.unlock();
				onDataChanged();
			}
			else
			{
				throw ElpidaException("Model",
					"Attempted to end a transaction that was not initiated or initiated from a different thread");
			}
		}
	};
}

#endif //APPS_QT_CORE_ABSTRACTIONS_MODEL_HPP
