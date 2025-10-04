/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 15/9/2024.
//

#ifndef ELPIDA_BASE64_HPP
#define ELPIDA_BASE64_HPP

#include <string>

namespace Elpida
{

	class Base64
	{
	public:
		static std::string Encode(const unsigned char* data, std::size_t size);
		static std::string Decode(const unsigned char* data, std::size_t size);
	};

} // Elpida

#endif //ELPIDA_BASE64_HPP
