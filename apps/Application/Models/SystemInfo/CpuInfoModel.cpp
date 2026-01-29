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

#include "CpuInfoModel.hpp"

#include <utility>

namespace Elpida::Application
{
	CpuInfoModel::CpuInfoModel(std::string architecture,
			std::string vendorName,
			std::string modelName)
			:
			_architecture(std::move(architecture)),
			_vendorName(std::move(vendorName)),
			_modelName(std::move(modelName))
	{
	}

	const std::string& CpuInfoModel::GetArchitecture() const
	{
		return _architecture;
	}

	const std::string& CpuInfoModel::GetVendorName() const
	{
		return _vendorName;
	}

	const std::string& CpuInfoModel::GetModelName() const
	{
		return _modelName;
	}
} // Application