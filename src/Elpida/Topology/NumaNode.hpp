//
// Created by klapeto on 5/3/2023.
//

#ifndef _NUMANODE_HPP_
#define _NUMANODE_HPP_

#include "Elpida/Topology/TopologyNode.hpp"
#include "Elpida/Topology/MemoryPageType.hpp"
#include "Elpida/Vector.hpp"
#include "Elpida/Size.hpp"
#include "Elpida/Optional.hpp"
#include "Elpida/Ref.hpp"

namespace Elpida
{

	class NumaNode : public TopologyNode
	{
	 public:
		[[nodiscard]]
		const Vector<MemoryPageType>& GetMemoryPageTypes() const;

		[[nodiscard]]
		Size GetLocalMemorySize() const;

		NumaNode(const NumaNode&) = delete;
		NumaNode(NumaNode&&) noexcept = delete;
		NumaNode& operator=(const NumaNode&) = delete;
		NumaNode& operator=(NumaNode&&) noexcept = delete;
		~NumaNode() override = default;
	 private:
		NumaNode(Optional<Ref<TopologyNode>> parent, const Vector<CpuKind>& cpuKinds, void* rootObj, void* node);

		Vector<MemoryPageType> _memoryPageTypes;
		Size _localMemorySize;

		friend class TopologyNode;
	};

} // Elpida

#endif //_NUMANODE_HPP_
