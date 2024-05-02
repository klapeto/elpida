//
// Created by klapeto on 14/3/2023.
//

#include "Elpida/Core/CpuInfo.hpp"

#include <utility>

namespace Elpida
{
	CpuInfo::CpuInfo(String architecture, String vendorName, String modelName)
		: _architecture(std::move(architecture)), _vendorName(std::move(vendorName)), _modelName(std::move(modelName))
	{
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
} // Elpida