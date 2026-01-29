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
// Created by klapeto on 4/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_CPUCACHENODE_HPP_
#define ELPIDA_TOPOLOGY_CPUCACHENODE_HPP_

#include "Elpida/Core/Topology/TopologyNode.hpp"
#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/Optional.hpp"
#include "Elpida/Core/Vector.hpp"

namespace Elpida
{
	enum class CacheType
	{
		Data,
		Instruction,
		Unified
	};

	class CpuCacheNode final : public TopologyNode
	{
	 public:
		[[nodiscard]]
		CacheType GetCacheType() const;

		[[nodiscard]]
		Size GetSize() const;

		[[nodiscard]]
		Size GetLineSize() const;

		[[nodiscard]]
		int GetAssociativitySets() const;

		[[nodiscard]]
		bool IsFullyAssociative() const;

		CpuCacheNode(
			NodeType type,
			CacheType cacheType,
			Size size,
			Size lineSize,
			int associativitySets,
			bool fullyAssociative);

		CpuCacheNode(const CpuCacheNode&) = delete;
		CpuCacheNode(CpuCacheNode&&) noexcept = delete;
		CpuCacheNode& operator=(const CpuCacheNode&) = delete;
		CpuCacheNode& operator=(CpuCacheNode&&) noexcept = delete;
		~CpuCacheNode() override = default;
	 private:
		bool _fullyAssociative;
		Size _size;
		Size _lineSize;
		CacheType _cacheType;
		int _associativitySets;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_CPUCACHENODE_HPP_
