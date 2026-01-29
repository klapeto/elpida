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
// Created by klapeto on 21/3/2023.
//

#ifndef ELPIDA_CPUINFOMODEL_HPP
#define ELPIDA_CPUINFOMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include <vector>
#include <unordered_map>
#include <string>

namespace Elpida::Application
{
	class CpuInfoModel : public Model
	{
	public:

		[[nodiscard]]
		const std::string& GetArchitecture() const;

		[[nodiscard]]
		const std::string& GetVendorName() const;

		[[nodiscard]]
		const std::string& GetModelName() const;

		CpuInfoModel() = default;
		CpuInfoModel(std::string architecture,
				std::string vendorName,
				std::string modelName);

		CpuInfoModel(CpuInfoModel&&) noexcept = default;
		CpuInfoModel& operator=(CpuInfoModel&&) noexcept = default;
		CpuInfoModel(const CpuInfoModel&) = delete;
		CpuInfoModel& operator=(const CpuInfoModel&) = delete;
		~CpuInfoModel() override = default;
	private:
		std::vector<std::string> _features;
		std::unordered_map<std::string, std::string> _additionalInfo;
		std::string _architecture;
		std::string _vendorName;
		std::string _modelName;
	};

} // Application

#endif //ELPIDA_CPUINFOMODEL_HPP
