//
// Created by klapeto on 4/3/2023.
//

#ifndef _ENVIRONMENTINFO_HPP_
#define _ENVIRONMENTINFO_HPP_

#include "Elpida/OverheadsInfo.hpp"
#include "Elpida/Topology/TopologyInfo.hpp"

namespace Elpida
{

	class EnvironmentInfo final
	{
	 public:
		[[nodiscard]]
		const TopologyInfo& GetTopologyInfo() const;

		[[nodiscard]]
		const OverheadsInfo& GetOverheadsInfo() const;

		EnvironmentInfo(const OverheadsInfo& overheadsInfo, TopologyInfo&& topologyInfo);
		EnvironmentInfo(const EnvironmentInfo&) = delete;
		EnvironmentInfo(EnvironmentInfo&&) noexcept = default;
		EnvironmentInfo& operator=(const EnvironmentInfo&) = delete;
		EnvironmentInfo& operator=(EnvironmentInfo&&) noexcept = default;
		~EnvironmentInfo() = default;
	 private:
		TopologyInfo _topologyInfo;
		OverheadsInfo _overheadsInfo;
	};

} // Elpida

#endif //_ENVIRONMENTINFO_HPP_
