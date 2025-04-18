//
// Created by klapeto on 11/3/2023.
//

#ifndef ELPIDA_TOPOLOGYLOADER_HPP_
#define ELPIDA_TOPOLOGYLOADER_HPP_

#include "Elpida/Core/Topology/TopologyInfo.hpp"

namespace Elpida
{

	class TopologyLoader final
	{
	 public:
		[[nodiscard]]
		static TopologyInfo LoadTopology();

		TopologyLoader() = delete;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGYLOADER_HPP_
