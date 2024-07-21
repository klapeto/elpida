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
