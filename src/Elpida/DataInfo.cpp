//
// Created by klapeto on 27/2/2023.
//

#include "DataInfo.hpp"

#include <utility>

namespace Elpida
{
	DataInfo::DataInfo(std::string name,
			std::string description,
			std::string unit,
			const std::vector<std::string>& metadata)
			: _name(std::move(name)),
			  _description(std::move(description)),
			  _unit(std::move(unit)),
			  _metadata(metadata),
			  _isUsed(true)
	{

	}

	DataInfo::DataInfo()
			: _isUsed(false)
	{

	}
} // Elpida