//
// Created by klapeto on 2/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_CPUKIND_HPP_
#define ELPIDA_TOPOLOGY_CPUKIND_HPP_

#include "Elpida/Core/String.hpp"
#include "Elpida/Core/Map.hpp"

namespace Elpida
{

	class CpuKind
	{
	 public:
		[[nodiscard]]
		const Map<String, String>& GetInfos() const;

		[[nodiscard]]
		int GetEfficiency() const;

		CpuKind(int efficiency, Map<String, String>&& infos);
		CpuKind(const CpuKind&) = delete;
		CpuKind(CpuKind&&) noexcept = default;
		CpuKind& operator=(const CpuKind&) = delete;
		CpuKind& operator=(CpuKind&&) = default;
		~CpuKind() = default;
	 private:
		Map<String, String> _infos;
		int _efficiency;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_CPUKIND_HPP_
