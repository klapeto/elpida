//
// Created by klapeto on 2/3/2023.
//

#ifndef _TOPOLOGYNODE_HPP_
#define _TOPOLOGYNODE_HPP_

#include <vector>
#include <optional>
#include <functional>
#include <variant>
#include <memory>
#include <unordered_map>

#include "Elpida/Topology/CpuKind.hpp"

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
		const std::vector<std::unique_ptr<TopologyNode>>& GetChildren() const;

		[[nodiscard]]
		const std::vector<std::unique_ptr<TopologyNode>>& GetMemoryChildren() const;

		[[nodiscard]]
		const std::vector<std::reference_wrapper<const TopologyNode>>& GetSiblings() const;

		[[nodiscard]]
		std::optional<std::reference_wrapper<const TopologyNode>> GetParent() const;

		[[nodiscard]]
		NodeType GetType() const;

		[[nodiscard]]
		std::optional<int> GetOsIndex() const;

		[[nodiscard]]
		std::optional<std::reference_wrapper<const TopologyNode>> GetLastAncestor(NodeType nodeTypes) const;

		TopologyNode(const TopologyNode&) = delete;
		TopologyNode(TopologyNode&&) noexcept = default;
		TopologyNode& operator=(const TopologyNode&) = delete;
		TopologyNode& operator=(TopologyNode&&) noexcept = default;
		virtual ~TopologyNode() = default;
	 private:
		std::vector<std::unique_ptr<TopologyNode>> _children;
		std::vector<std::unique_ptr<TopologyNode>> _memoryChildren;
		std::vector<std::reference_wrapper<const TopologyNode>> _siblings;

		std::optional<std::reference_wrapper<const TopologyNode>> _parent;

		NodeType _type;
		std::optional<int> _osIndex;

	 protected:
		TopologyNode(std::optional<std::reference_wrapper<TopologyNode>> parent, const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node);
		void addSibling(TopologyNode& node);

		void loadMachine(void* node);
		void loadPackage(void* node);
		void loadDie(void* node);
		void loadNumaNode(void* node);
		void loadGroup(void* node);
		void loadCore(void* node);
		void loadCache(void* node);
		void loadProcessingUnit(void* node);

		void loadChildren(const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node);
		void loadSiblings();
		void loadParents(std::optional<std::reference_wrapper<TopologyNode>> parent);

		static std::unique_ptr<TopologyNode> Load(std::optional<std::reference_wrapper<TopologyNode>> parent, const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node);

		friend class TopologyInfo;
	};

} // Elpida

#endif //_TOPOLOGYNODE_HPP_
