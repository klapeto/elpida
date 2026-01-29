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
// Created by klapeto on 2/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_TOPOLOGYNODE_HPP_
#define ELPIDA_TOPOLOGY_TOPOLOGYNODE_HPP_

#include "Elpida/Core/Topology/CpuKind.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Optional.hpp"
#include "Elpida/Core/Ref.hpp"

namespace Elpida
{
	enum NodeType
	{
		Machine = 0,
		Package = 1,
		NumaDomain = 1 << 1,
		Group = 1 << 2,
		Die = 1 << 3,
		Core = 1 << 4,
		L1ICache = 1 << 5,
		L1DCache = 1 << 6,
		L2ICache = 1 << 7,
		L2DCache = 1 << 8,
		L3ICache = 1 << 9,
		L3DCache = 1 << 10,
		L4Cache = 1 << 11,
		L5Cache = 1 << 12,
		ProcessingUnit = 1 << 13,
		Unknown = -1
	};

	class TopologyNode
	{
	 public:
		[[nodiscard]]
		const Vector<UniquePtr<TopologyNode>>& GetChildren() const;

		[[nodiscard]]
		Vector<UniquePtr<TopologyNode>>& GetChildren();

		[[nodiscard]]
		const Vector<UniquePtr<TopologyNode>>& GetMemoryChildren() const;

		[[nodiscard]]
		Vector<UniquePtr<TopologyNode>>& GetMemoryChildren();

		[[nodiscard]]
		const Vector<Ref<const TopologyNode>>& GetSiblings() const;

		[[nodiscard]]
		Optional<Ref<const TopologyNode>> GetParent() const;

		[[nodiscard]]
		NodeType GetType() const;

		[[nodiscard]]
		Optional<unsigned int> GetOsIndex() const;

		[[nodiscard]]
		Optional<Ref<const TopologyNode>> GetLastAncestor(NodeType nodeTypes) const;

		void SetParent(Ref<const TopologyNode> parent);
		void SetOsIndex(unsigned int index);
		void LoadSiblings();

		explicit TopologyNode(NodeType type);
		TopologyNode(const TopologyNode&) = delete;
		TopologyNode(TopologyNode&&) noexcept = default;
		TopologyNode& operator=(const TopologyNode&) = delete;
		TopologyNode& operator=(TopologyNode&&) noexcept = default;
		virtual ~TopologyNode() = default;
	 private:
		Vector<UniquePtr<TopologyNode>> _children;
		Vector<UniquePtr<TopologyNode>> _memoryChildren;
		Vector<Ref<const TopologyNode>> _siblings;

		Optional<Ref<const TopologyNode>> _parent;
		Optional<unsigned int> _osIndex;
		NodeType _type;
	 protected:
		void AddSibling(TopologyNode& node);
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_TOPOLOGYNODE_HPP_
