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
		CacheType _cacheType;
		Size _size;
		Size _lineSize;
		int _associativitySets;
		bool _fullyAssociative;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_CPUCACHENODE_HPP_
