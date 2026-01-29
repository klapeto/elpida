/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
