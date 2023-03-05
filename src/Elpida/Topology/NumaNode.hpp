//
// Created by klapeto on 5/3/2023.
//

#ifndef _NUMANODE_HPP_
#define _NUMANODE_HPP_

#include "TopologyNode.hpp"
#include "MemoryPageType.hpp"

#include <cstdlib>
#include <vector>

namespace Elpida
{

	class NumaNode : public TopologyNode
	{
	 public:
		[[nodiscard]]
		const std::vector<MemoryPageType>& GetMemoryPageTypes() const;

		[[nodiscard]]
		std::size_t GetLocalMemorySize() const;

		NumaNode(const NumaNode&) = delete;
		NumaNode(NumaNode&&) noexcept = delete;
		NumaNode& operator=(const NumaNode&) = delete;
		NumaNode& operator=(NumaNode&&) noexcept = delete;
		~NumaNode() override = default;
	 private:
		NumaNode(std::optional<std::reference_wrapper<TopologyNode>> parent, const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node);

		std::vector<MemoryPageType> _memoryPageTypes;
		std::size_t _localMemorySize;

		friend class TopologyNode;
	};

} // Elpida

#endif //_NUMANODE_HPP_
