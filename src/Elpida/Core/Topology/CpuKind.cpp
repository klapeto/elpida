//
// Created by klapeto on 2/3/2023.
//

#include "Elpida/Core/Topology/CpuKind.hpp"

namespace Elpida
{
	CpuKind::CpuKind(int efficiency, Map<String, String>&& infos)
		: _infos(std::move(infos)), _efficiency(efficiency)
	{

	}

	int CpuKind::GetEfficiency() const
	{
		return _efficiency;
	}

	const Map<String, String>& CpuKind::GetInfos() const
	{
		return _infos;
	}
} // Elpida