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
// Created by klapeto on 8/4/2023.
//

#include "OsInfoModel.hpp"

#include <utility>

namespace Elpida::Application
{
	OsInfoModel::OsInfoModel(std::string category, std::string name, std::string version)
		: _category(std::move(category)), _name(std::move(name)), _version(std::move(version))
	{
	}
	const std::string& OsInfoModel::GetCategory() const
	{
		return _category;
	}
	const std::string& OsInfoModel::GetName() const
	{
		return _name;
	}
	const std::string& OsInfoModel::GetVersion() const
	{
		return _version;
	}
} // Application