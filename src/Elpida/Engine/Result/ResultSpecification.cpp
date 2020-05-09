//
// Created by klapeto on 8/5/20.
//

#include "Elpida/Engine/Result/ResultSpecification.hpp"

#include <utility>

namespace Elpida
{

	ResultSpecification::ResultSpecification(std::string name,
		std::string unit,
		Type type,
		AggregationType aggregationType,
		std::string description)
		: _name(std::move(name)), _unit(std::move(unit)), _description(std::move(description)), _type(type),
		  _aggregationType(aggregationType)
	{
	}
}