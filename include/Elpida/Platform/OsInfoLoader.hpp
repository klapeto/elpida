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
// Created by klapeto on 13/3/2023.
//

#ifndef ELPIDA_OSINFOLOADER_HPP_
#define ELPIDA_OSINFOLOADER_HPP_

#include "Elpida/Core/OsInfo.hpp"
namespace Elpida
{

	class OsInfoLoader final
	{
	 public:
		static OsInfo Load();
		OsInfoLoader() = delete;
	};

} // Elpida

#endif //ELPIDA_OSINFOLOADER_HPP_
