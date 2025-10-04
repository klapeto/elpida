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

#include "Elpida/Core/OsInfo.hpp"

#include <utility>

namespace Elpida
{
	OsInfo::OsInfo(String category, String name, String version)
		: _category(std::move(category)), _name(std::move(name)), _version(std::move(version))
	{
	}
	const String& OsInfo::GetCategory() const
	{
		return _category;
	}
	const String& OsInfo::GetName() const
	{
		return _name;
	}
	const String& OsInfo::GetVersion() const
	{
		return _version;
	}
} // Elpida