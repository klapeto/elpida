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

#include "Elpida/Utilities/Event.hpp"
#include "Elpida/Common/ElpidaException.hpp"

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
