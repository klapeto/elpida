/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 5/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_PROCESSINGUNITNODE_HPP_
#define ELPIDA_TOPOLOGY_PROCESSINGUNITNODE_HPP_

#include "Elpida/Core/Topology/TopologyNode.hpp"
#include "Elpida/Core/Topology/NumaNode.hpp"
#include "Elpida/Core/Topology/CpuCacheNode.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/Optional.hpp"

namespace Elpida
{

	class ProcessingUnitNode final : public TopologyNode
	{
	 public:

		[[nodiscard]]
		const NumaNode& GetNumaNode() const;

		[[nodiscard]]
		Optional<Ref<const CpuKind>> GetCpuKind() const;

		[[nodiscard]]
		Optional<Ref<const CpuCacheNode>> GetLastCache() const;

		void SetLastCache(const CpuCacheNode& cacheNode);
		void SetNumaNode(const NumaNode& cacheNode);
		void SetCpuKind(const CpuKind& cacheNode);

		explicit ProcessingUnitNode();
		ProcessingUnitNode(const ProcessingUnitNode&) = delete;
		ProcessingUnitNode(ProcessingUnitNode&&) noexcept = delete;
		ProcessingUnitNode& operator=(const ProcessingUnitNode&) = delete;
		ProcessingUnitNode& operator=(ProcessingUnitNode&&) noexcept = delete;
		~ProcessingUnitNode() override = default;
	 private:
		Optional<Ref<const NumaNode>> _numaNode;
		Optional<Ref<const CpuCacheNode>> _lastCache;
		Optional<Ref<const CpuKind>> _cpuKind;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_PROCESSINGUNITNODE_HPP_
