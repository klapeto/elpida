/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 13/4/20.
//

#ifndef APPS_QT_CORE_ABSTRACTIONS_MODEL_HPP
#define APPS_QT_CORE_ABSTRACTIONS_MODEL_HPP

#include "Event.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include <mutex>

namespace Elpida::Application
{

	class Model
	{
	 public:
		Event<>& DataChanged()
		{
			return _dataChanged;
		}

		template<typename T, typename TCallable>
		void Transactional(TCallable callable)
		{
			static_assert(std::is_base_of_v<Model, T>, "Only derived classes can be used for template parameter");
			try
			{
				BeginUpdateTransaction();
				callable((T&)*this);
				EndUpdateTransaction();
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
	 private:
		Event<> _dataChanged;
		std::mutex _mutex;
		std::unique_lock<std::mutex> _lock;
		bool _transaction;

		void OnDataChanged()
		{
			if (!_transaction && !_lock.owns_lock())
			{
				_dataChanged.Raise();
			}
		}

		void BeginUpdateTransaction()
		{
			_lock.lock();
			_transaction = true;
		}

		void EndUpdateTransaction()
		{
			if (_lock.owns_lock())
			{
				_transaction = false;
				_lock.unlock();
				OnDataChanged();
			}
			else
			{
				throw ElpidaException("Attempted to end a transaction that was not initiated or initiated from a different thread");
			}
		}
	};
}

#endif //APPS_QT_CORE_ABSTRACTIONS_MODEL_HPP
