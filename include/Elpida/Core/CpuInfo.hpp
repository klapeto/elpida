//
// Created by klapeto on 14/3/2023.
//

#ifndef ELPIDA_CPUINFO_HPP_
#define ELPIDA_CPUINFO_HPP_

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Map.hpp"
#include "Elpida/Core/String.hpp"

namespace Elpida
{

	class CpuInfo final
	{
	 public:
		[[nodiscard]]
		const Vector<String>& GetFeatures() const;

		[[nodiscard]]
		const Map<String, String>& GetAdditionalInfo() const;

		[[nodiscard]]
		const String& GetArchitecture() const;

		[[nodiscard]]
		const String& GetVendorName() const;

		[[nodiscard]]
		const String& GetModelName() const;

		[[nodiscard]]
		double GetFrequency() const;

		CpuInfo(String architecture, String vendorName, String modelName, double frequency, Vector<String>&& features, Map<String, String>&& additionalInfo);
		CpuInfo(const CpuInfo&) = default;
		CpuInfo(CpuInfo&&) noexcept = default;
		CpuInfo& operator=(const CpuInfo&) = default;
		CpuInfo& operator=(CpuInfo&&) noexcept = default;
		~CpuInfo() = default;
	 private:
		Vector<String> _features;
		Map<String, String> _additionalInfo;
		String _architecture;
		String _vendorName;
		String _modelName;
		double _frequency;
	};

} // Elpida

#endif //ELPIDA_CPUINFO_HPP_
