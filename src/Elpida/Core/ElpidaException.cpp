//
// Created by klapeto on 2/3/2023.
//

#include "Elpida/Core/ElpidaException.hpp"

namespace Elpida
{
	ElpidaException::ElpidaException(const String& arg)
		: runtime_error(arg)
	{
	}
} // Elpida