//
// Created by klapeto on 20/12/2023.
//

#ifndef PARSEEXCEPTION_HPP
#define PARSEEXCEPTION_HPP
#include <Elpida/Core/ElpidaException.hpp>

namespace Elpida
{
	class ParseException final : public ElpidaException
	{
	public:
		explicit ParseException(std::string_view message)
			: ElpidaException((message))
		{
		}

		template<typename  T>
		ParseException(std::string_view unexpected, T expected)
			: ElpidaException("Unexpected , ", unexpected, ":expected ", expected)
		{
		}

		template<typename  T>
		ParseException(char unexpectedCharacter, T expected)
			: ElpidaException("Unexpected character ('", unexpectedCharacter, "'): expected '", expected, "'")
		{
		}
	};
} // Elpida

#endif //PARSEEXCEPTION_HPP
