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

#ifndef ELPIDA_TOPOLOGY_NUMANODE_HPP_
#define ELPIDA_TOPOLOGY_NUMANODE_HPP_

#include "Elpida/Core/Topology/TopologyNode.hpp"
#include "Elpida/Core/Topology/MemoryPageType.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/Optional.hpp"
#include "Elpida/Core/Ref.hpp"

namespace Elpida
{

	class NumaNode final : public TopologyNode
	{
	 public:
		[[nodiscard]]
		const Vector<MemoryPageType>& GetMemoryPageTypes() const;

		[[nodiscard]]
		Size GetLocalMemorySize() const;

		NumaNode(Vector<MemoryPageType>&& memoryPageTypes, Size localMemorySize);
		NumaNode(const NumaNode&) = delete;
		NumaNode(NumaNode&&) noexcept = delete;
		NumaNode& operator=(const NumaNode&) = delete;
		NumaNode& operator=(NumaNode&&) noexcept = delete;
		~NumaNode() override = default;
	 private:
		Vector<MemoryPageType> _memoryPageTypes;
		Size _localMemorySize;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_NUMANODE_HPP_
