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
				_transaction = false;
				throw;
			}
		}

		Model();
		Model(const Model&) = delete;
		Model(Model&&) noexcept = default;
		Model& operator=(const Model&) = delete;
		Model& operator=(Model&&) noexcept = default;
		virtual ~Model() = default;
	protected:
		void OnDataChanged();
	private:
		Event<> _dataChanged;
		bool _transaction;

		void BeginUpdateTransaction();
		void EndUpdateTransaction();
	};
}

#endif //APPS_QT_CORE_ABSTRACTIONS_MODEL_HPP
