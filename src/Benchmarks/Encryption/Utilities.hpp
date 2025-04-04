//
// Created by klapeto on 11/8/2024.
//

#ifndef ELPIDA_UTILITIES_HPP
#define ELPIDA_UTILITIES_HPP

#include <openssl/err.h>
#include "Elpida/Core/ElpidaException.hpp"

namespace Elpida
{

	class Utilities
	{
	public:
		static void ThrowOpenSSLError(const char* message)
		{
			char buff[512];
			ERR_error_string_n(ERR_get_error(), buff, sizeof(buff));
			throw ElpidaException(message, buff);
		}
	};

} // Elpida

#endif //ELPIDA_UTILITIES_HPP
