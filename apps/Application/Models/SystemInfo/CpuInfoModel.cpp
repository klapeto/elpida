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