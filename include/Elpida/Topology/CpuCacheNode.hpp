//
// Created by klapeto on 4/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_CPUCACHENODE_HPP_
#define ELPIDA_TOPOLOGY_CPUCACHENODE_HPP_

#include "Elpida/Topology/TopologyNode.hpp"
#include "Elpida/Size.hpp"
#include "Elpida/Optional.hpp"
#include "Elpida/Vector.hpp"

namespace Elpida
{
	enum class CacheType
	{
		Data,
		Instruction,
		Unified
	};

	class CpuCacheNode : public TopologyNode
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

		CpuCacheNode(const CpuCacheNode&) = delete;
		CpuCacheNode(CpuCacheNode&&) noexcept = delete;
		CpuCacheNode& operator=(const CpuCacheNode&) = delete;
		CpuCacheNode& operator=(CpuCacheNode&&) noexcept = delete;
		~CpuCacheNode() override = default;
	 private:
		CpuCacheNode(Optional<Ref<TopologyNode>> parent, const Vector<CpuKind>& cpuKinds, void* rootObj, void* node);

		CacheType _cacheType;
		Size _size;
		Size _lineSize;
		int _associativitySets;
		bool _fullyAssociative;

		friend class TopologyNode;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_CPUCACHENODE_HPP_
