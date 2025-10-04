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
// Created by klapeto on 20/12/2023.
//

#ifndef ELPIDA_XML_PARSEEXCEPTION_HPP
#define ELPIDA_XML_PARSEEXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace Elpida
{
	class ParseException final : public std::runtime_error
	{
	public:
		explicit ParseException(const std::string& message)
			: std::runtime_error(message)
		{
		}

		template<typename TUnexpected, typename TExpected>
		ParseException(TUnexpected unexpected, TExpected expected)
			: ParseException(std::string("Unexpected , ") + unexpected + ":expected " + expected)
		{
		}

		template<typename  T>
		ParseException(char unexpectedCharacter, T expected)
			: ParseException(std::string("Unexpected character ('") +  unexpectedCharacter +  "'): expected '" + expected + "'")
		{
		}
	};
} // Elpida

#endif //ELPIDA_XML_PARSEEXCEPTION_HPP
