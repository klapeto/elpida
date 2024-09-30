//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 29/9/2024.
//

#ifndef ELPIDA_INFOGETTER_HPP
#define ELPIDA_INFOGETTER_HPP

#include <string>

namespace Elpida
{

	class InfoGetter
	{
	public:
		static std::string GetInfoData(const std::string& benchmarksPath);
	};

} // Elpida

#endif //ELPIDA_INFOGETTER_HPP
