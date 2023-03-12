//
// Created by klapeto on 12/3/2023.
//


#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/OsUtilities.hpp"

#include <cerrno>
#include <cstring>

namespace Elpida
{

	String OsUtilities::GetLastErrorString()
	{
		return { strerror(errno) };
	}

} // Elpida


#endif // defined(ELPIDA_UNIX)