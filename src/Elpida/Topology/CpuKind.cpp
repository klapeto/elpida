//
// Created by klapeto on 2/3/2023.
//

#include "Elpida/Topology/CpuKind.hpp"

namespace Elpida
{
	CpuKind::CpuKind(int efficiency, Map<String, String>&& infos)
		: _infos(std::move(infos)), _efficiency(efficiency)
	{

	}

	int CpuKind::getEfficiency() const
	{
		return _efficiency;
	}

	const Map<String, String>& CpuKind::getInfos() const
	{
		return _infos;
	}
} // Elpida