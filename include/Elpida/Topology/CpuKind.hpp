//
// Created by klapeto on 2/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_CPUKIND_HPP_
#define ELPIDA_TOPOLOGY_CPUKIND_HPP_

#include "Elpida/String.hpp"
#include "Elpida/Map.hpp"

namespace Elpida
{

	class CpuKind
	{
	 public:
		[[nodiscard]]
		const Map<String, String>& getInfos() const;

		[[nodiscard]]
		int getEfficiency() const;

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