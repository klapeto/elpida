//
// Created by klapeto on 9/6/2024.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS) &&  defined(_M_ARM64)

namespace Elpida{

	CpuInfo CpuInfoLoader::Load()
	{
		std::string vendor;
		try
		{
			vendor = ReadRegistryKeyFromHKLM(R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", "VendorIdentifier");
		}
		catch (const std::exception&)
		{
			vendor = "Unknown Vendor";
		}

		std::string modelName;
		try
		{
			modelName = ReadRegistryKeyFromHKLM(R"(HARDWARE\DESCRIPTION\System\CentralProcessor\0)", "ProcessorNameString");
		}
		catch (const std::exception&)
		{
			modelName = "Unknown Model";
		}

		return CpuInfo("AArch64", std::move(vendor), std::move(modelName));
	}
}

#endif