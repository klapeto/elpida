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

#ifndef ELPIDA_OSINFOMODEL_HPP_
#define ELPIDA_OSINFOMODEL_HPP_

#include <string>

#include "Models/Abstractions/Model.hpp"

namespace Elpida::Application
{

	class OsInfoModel: public Model
	{
	 public:

		[[nodiscard]]
		const std::string& GetCategory() const;

		[[nodiscard]]
		const std::string& GetName() const;

		[[nodiscard]]
		const std::string& GetVersion() const;

		OsInfoModel() = default;
		OsInfoModel(std::string category, std::string name, std::string version);
		OsInfoModel(const OsInfoModel&) = delete;
		OsInfoModel(OsInfoModel&&) = default;
		OsInfoModel& operator=(const OsInfoModel&) = delete;
		OsInfoModel& operator=(OsInfoModel&&) = default;
		~OsInfoModel() override = default;
	 private:
		std::string _category;
		std::string _name;
		std::string _version;
	};

} // Application

#endif //ELPIDA_OSINFOMODEL_HPP_
