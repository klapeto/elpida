//
// Created by klapeto on 8/5/20.
//

#include "Elpida/Engine/Data/DataSpecification.hpp"

#include <utility>

namespace Elpida
{

	DataSpecification::DataSpecification(std::string name, std::string unit, std::string description)
		: DataSpecification(name, unit, 8, description)
	{

	}
	DataSpecification::DataSpecification(std::string name,
		std::string unit,
		size_t sizeShouldBeDivisibleBy,
		std::string description)
		: _name(std::move(name)), _description(std::move(description)), _unit(std::move(unit)),
		  _sizeShouldBeDivisibleBy(sizeShouldBeDivisibleBy)
	{

	}
}