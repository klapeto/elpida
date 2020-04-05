/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * OutOfRangeException.hpp
 *
 *  Created on: 29 Ιουλ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_EXCEPTIONS_OUTOFRANGEEXCEPTION_HPP_
#define ELPIDA_EXCEPTIONS_OUTOFRANGEEXCEPTION_HPP_

#include <string>

#include "Elpida/Exceptions/ElpidaException.hpp"

namespace Elpida
{

	class OutOfRangeException final : public ElpidaException
	{
	public:
		OutOfRangeException()
			: ElpidaException()
		{

		}

		OutOfRangeException(const std::string& what)
			: ElpidaException(what)
		{

		}

		OutOfRangeException(const std::string& what, const std::string& message)
			: ElpidaException(what, message)
		{

		}

		~OutOfRangeException()
		{

		}
	};

} /* namespace Elpida */

#endif /* ELPIDA_EXCEPTIONS_OUTOFRANGEEXCEPTION_HPP_ */
