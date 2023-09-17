//
// Created by klapeto on 14/3/2023.
//

#include "Elpida/Core/CpuInfo.hpp"

#include <utility>

namespace Elpida
{
	CpuInfo::CpuInfo(String architecture, String vendorName, String modelName, double frequency, Vector<String>&& features, Map<String, String>&& additionalInfo)
		: _features(std::move(features)), _additionalInfo(std::move(additionalInfo)), _architecture(std::move(architecture)), _vendorName(std::move(vendorName)), _modelName(std::move(modelName)), _frequency(frequency)
	{
	}

	const Vector<String>& CpuInfo::GetFeatures() const
	{
		return _features;
	}

	const Map<String, String>& CpuInfo::GetAdditionalInfo() const
	{
		return _additionalInfo;
	}

	const String& CpuInfo::GetArchitecture() const
	{
		return _architecture;
	}

	const String& CpuInfo::GetVendorName() const
	{
		return _vendorName;
	}

	const String& CpuInfo::GetModelName() const
	{
		return _modelName;
	}
	double CpuInfo::GetFrequency() const
	{
		return _frequency;
	}
} // Elpida