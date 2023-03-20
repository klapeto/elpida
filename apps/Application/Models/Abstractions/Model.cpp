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

	void Model::OnDataChanged()
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

}