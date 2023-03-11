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

	class NumaNode : public TopologyNode
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

		friend class TopologyNode;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_NUMANODE_HPP_
