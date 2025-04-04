//
// Created by klapeto on 14/3/2023.
//

#ifndef ELPIDA_CPUINFOLOADER_HPP_
#define ELPIDA_CPUINFOLOADER_HPP_

#include "Elpida/Core/CpuInfo.hpp"
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/Size.hpp"

namespace Elpida
{

	class CpuInfoLoader final
	{
	 public:
		static CpuInfo Load();
		CpuInfoLoader() = delete;
	};

} // Elpida

#endif //ELPIDA_CPUINFOLOADER_HPP_
