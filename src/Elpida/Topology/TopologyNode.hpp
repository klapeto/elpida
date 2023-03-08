//
// Created by klapeto on 2/3/2023.
//

#ifndef _TOPOLOGYNODE_HPP_
#define _TOPOLOGYNODE_HPP_


#include "Elpida/Topology/CpuKind.hpp"
#include "Elpida/Vector.hpp"
#include "Elpida/UniquePtr.hpp"
#include "Elpida/Optional.hpp"
#include "Elpida/Ref.hpp"

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
		const Vector<UniquePtr<TopologyNode>>& GetMemoryChildren() const;

		[[nodiscard]]
		const Vector<Ref<const TopologyNode>>& GetSiblings() const;

		[[nodiscard]]
		Optional<Ref<const TopologyNode>> GetParent() const;

		[[nodiscard]]
		NodeType GetType() const;

		[[nodiscard]]
		Optional<int> GetOsIndex() const;

		[[nodiscard]]
		Optional<Ref<const TopologyNode>> GetLastAncestor(NodeType nodeTypes) const;

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

		NodeType _type;
		Optional<int> _osIndex;
	 protected:
		TopologyNode(Optional<Ref<TopologyNode>> parent, const Vector<CpuKind>& cpuKinds, void* rootObj, void* node);
		void addSibling(TopologyNode& node);

		void loadMachine(void* node);
		void loadPackage(void* node);
		void loadDie(void* node);
		void loadNumaNode(void* node);
		void loadGroup(void* node);
		void loadCore(void* node);
		void loadCache(void* node);
		void loadProcessingUnit(void* node);

		void loadChildren(const Vector<CpuKind>& cpuKinds, void* rootObj, void* node);
		void loadSiblings();
		void loadParents(Optional<Ref<TopologyNode>> parent);

		static UniquePtr<TopologyNode> Load(Optional<Ref<TopologyNode>> parent, const Vector<CpuKind>& cpuKinds, void* rootObj, void* node);

		friend class TopologyInfo;
	};

} // Elpida

#endif //_TOPOLOGYNODE_HPP_
