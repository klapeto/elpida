/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 20/3/2023.
//

#include "Model.hpp"

namespace Elpida::Application
{

	Model::Model()
		: _transaction(false)
	{

	}

	void Model::OnDataChanged() const
	{
		if (!_transaction)
		{
			_dataChanged.Raise();
		}
	}

	void Model::EndUpdateTransaction()
	{
		_transaction = false;
		OnDataChanged();
	}

	void Model::BeginUpdateTransaction()
	{
		_transaction = true;
	}

	Model::Model(const Model&)
			: _transaction(false)
	{

	}

	Model& Model::operator=(const Model&)
	{
		return *this;
	}
}