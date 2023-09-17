//
// Created by klapeto on 21/3/2023.
//

#include "CpuInfoModel.hpp"

#include <utility>

namespace Elpida::Application
{
	CpuInfoModel::CpuInfoModel(std::string architecture,
		std::string vendorName,
		std::string modelName,
		double frequency,
		std::vector<std::string> features,
		std::unordered_map<std::string, std::string> additionalInfo)
		: _features(std::move(features)),
		  _additionalInfo(std::move(additionalInfo)),
		  _architecture(std::move(architecture)),
		  _vendorName(std::move(vendorName)),
		  _modelName(std::move(modelName)),
		  _frequency(frequency)
	{
	}

	const std::vector<std::string>& CpuInfoModel::GetFeatures() const
	{
		return _features;
	}

	const std::unordered_map<std::string, std::string>& CpuInfoModel::GetAdditionalInfo() const
	{
		return _additionalInfo;
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

	double CpuInfoModel::GetFrequency() const
	{
		return _frequency;
	}
} // Application