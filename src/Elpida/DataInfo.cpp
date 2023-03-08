//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/DataInfo.hpp"

#include <utility>

namespace Elpida
{
	DataInfo::DataInfo(String name,
			String description,
			String unit,
			const Vector<String>& metadata)
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