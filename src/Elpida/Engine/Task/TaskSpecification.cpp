//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Utilities/Uuid.hpp"
#include "Elpida/Engine/Data/DataPropertiesTransformer.hpp"

#include <utility>

namespace Elpida
{
	TaskSpecification::TaskSpecification(std::string name, ResultSpecification resultSpecification)
		: _name(std::move(name)),
		  _resultSpecification(std::move(resultSpecification)),
		  _dataPropertiesTransformer(nullptr),
		  _acceptsInput(false),
		  _exportsOutput(false)
	{
		_id = Uuid::create();
	}

	TaskSpecification::~TaskSpecification()
	{
		for (auto configSpec: _configurationSpecifications)
		{
			delete configSpec;
		}
		delete _dataPropertiesTransformer;
	}
}