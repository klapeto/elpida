//
// Created by klapeto on 8/5/20.
//

#include "Elpida/Engine/Data/DataSpecification.hpp"

#include <utility>

namespace Elpida
{

	DataSpecification::DataSpecification(std::string name, std::string unit, std::string description)
		: DataSpecification(std::move(name), std::move(unit), 8, std::move(description))
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

	DataSpecification::DataSpecification()
		: _sizeShouldBeDivisibleBy(1)
	{

	}
}