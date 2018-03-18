/**************************************************************************
*   elpida - CPU benchmark tool
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
 * ElpidaException.hpp
 *
 *  Created on: 13 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_UTILITIES_ELPIDAEXCEPTION_HPP_
#define SRC_UTILITIES_ELPIDAEXCEPTION_HPP_

#include <string>
namespace Elpida
{

	class ElpidaException
	{
		public:

			const std::string& getMessage() const
			{
				return _message;
			}

			const std::string& getWhatThrewThis() const
			{
				return _what;
			}

			ElpidaException()
			{

			}

			ElpidaException(const std::string& what) :
					_what(what)
			{

			}

			ElpidaException(const std::string& what, const std::string& message) :
					_what(what), _message(message)
			{

			}

			~ElpidaException()
			{

			}

			ElpidaException(ElpidaException&&) = default;
			ElpidaException(const ElpidaException&) = default;
			ElpidaException& operator=(ElpidaException&&) = default;
			ElpidaException& operator=(const ElpidaException&) = default;

		protected:
			std::string _what;
			std::string _message;
	};

} /* namespace Elpida */

#endif /* SRC_UTILITIES_ELPIDAEXCEPTION_HPP_ */
