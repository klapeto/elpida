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
