//
// Created by klapeto on 2/3/2023.
//

#include "ElpidaException.hpp"

namespace Elpida
{
	ElpidaException::ElpidaException(const std::string& arg)
		: runtime_error(arg)
	{
	}
} // Elpida