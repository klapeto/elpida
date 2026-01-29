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
// Created by klapeto on 2/3/2023.
//

#ifndef ELPIDA_ELPIDAEXCEPTION_HPP_
#define ELPIDA_ELPIDAEXCEPTION_HPP_

#include <stdexcept>

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

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

#endif //ELPIDA_ELPIDAEXCEPTION_HPP_
