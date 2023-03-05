//
// Created by klapeto on 4/3/2023.
//

#ifndef _CPUCACHENODE_HPP_
#define _CPUCACHENODE_HPP_

#include "TopologyNode.hpp"

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
		std::size_t GetSize() const;

		[[nodiscard]]
		std::size_t GetLineSize() const;

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
		CpuCacheNode(std::optional<std::reference_wrapper<TopologyNode>> parent, const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node);

		CacheType _cacheType;
		std::size_t _size;
		std::size_t _lineSize;
		int _associativitySets;
		bool _fullyAssociative;

		friend class TopologyNode;
	};

} // Elpida

#endif //_CPUCACHENODE_HPP_
