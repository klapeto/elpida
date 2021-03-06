/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

#ifndef ELPIDA_EXCEPTIONS_ELPIDAEXCEPTION_HPP_
#define ELPIDA_EXCEPTIONS_ELPIDAEXCEPTION_HPP_

#include <exception>
#include <string>

namespace Elpida
{

	class ElpidaException : public std::exception
	{
	public:

		[[nodiscard]] const std::string& getComponent() const
		{
			return _component;
		}

		[[nodiscard]] const char* what() const noexcept override
		{
			return _message.c_str();
		}

		ElpidaException() = default;

		explicit ElpidaException(std::string message)
			: _message(std::move(message))
		{

		}

		ElpidaException(std::string component, std::string message)
			: _component(std::move(component)), _message(std::move(message))
		{

		}

		~ElpidaException() override = default;

		ElpidaException(ElpidaException&&) = default;
		ElpidaException(const ElpidaException&) = default;
		ElpidaException& operator=(ElpidaException&&) = default;
		ElpidaException& operator=(const ElpidaException&) = default;

	protected:
		std::string _component;
		std::string _message;
	};

} /* namespace Elpida */

#endif /* ELPIDA_EXCEPTIONS_ELPIDAEXCEPTION_HPP_ */
