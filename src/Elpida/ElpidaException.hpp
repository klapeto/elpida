//
// Created by klapeto on 2/3/2023.
//

#ifndef _ELPIDAEXCEPTION_HPP_
#define _ELPIDAEXCEPTION_HPP_

#include <stdexcept>

#include "Elpida/String.hpp"
#include "Elpida/ValueUtilities.hpp"

namespace Elpida
{

	class ElpidaException : public std::runtime_error
	{
	 public:

		template<typename ... T>
		explicit ElpidaException(T&& ... args):
			ElpidaException((const String&)ValueUtilities::Cs(std::forward<T>(args)...))
		{
		}
		explicit ElpidaException(const String& arg);
		ElpidaException(const ElpidaException&) = default;
		ElpidaException(ElpidaException&&) noexcept = default;
		ElpidaException& operator=(const ElpidaException&) = default;
		ElpidaException& operator=(ElpidaException&&) noexcept = default;
		~ElpidaException() override = default;
	};

} // Elpida

#endif //_ELPIDAEXCEPTION_HPP_
