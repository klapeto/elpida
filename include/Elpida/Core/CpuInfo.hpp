//
// Created by klapeto on 14/3/2023.
//

#ifndef ELPIDA_CPUINFO_HPP_
#define ELPIDA_CPUINFO_HPP_

#include "Elpida/Core/String.hpp"

namespace Elpida
{

	class CpuInfo final
	{
	 public:
		[[nodiscard]]
		const String& GetArchitecture() const;

		[[nodiscard]]
		const String& GetVendorName() const;

		[[nodiscard]]
		const String& GetModelName() const;

		CpuInfo(String architecture, String vendorName, String modelName);
		CpuInfo(const CpuInfo&) = default;
		CpuInfo(CpuInfo&&) noexcept = default;
		CpuInfo& operator=(const CpuInfo&) = default;
		CpuInfo& operator=(CpuInfo&&) noexcept = default;
		~CpuInfo() = default;
	 private:
		String _architecture;
		String _vendorName;
		String _modelName;
	};

} // Elpida

#endif //ELPIDA_CPUINFO_HPP_
