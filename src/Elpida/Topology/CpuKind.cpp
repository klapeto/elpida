//
// Created by klapeto on 2/3/2023.
//

#include "CpuKind.hpp"

namespace Elpida
{
	CpuKind::CpuKind(int efficiency, std::unordered_map<std::string, std::string>&& infos)
		: _infos(std::move(infos)), _efficiency(efficiency)
	{

	}

	int CpuKind::getEfficiency() const
	{
		return _efficiency;
	}

	const std::unordered_map<std::string, std::string>& CpuKind::getInfos() const
	{
		return _infos;
	}
} // Elpida