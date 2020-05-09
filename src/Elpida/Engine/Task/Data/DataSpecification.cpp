//
// Created by klapeto on 8/5/20.
//

#include "Elpida/Engine/Task/Data/DataSpecification.hpp"

#include <utility>

namespace Elpida
{

	DataSpecification::DataSpecification(std::string name, std::string unit, std::string description)
		: _name(std::move(name)), _description(std::move(description)), _unit(std::move(unit))
	{

	}
}