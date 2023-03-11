//
// Created by klapeto on 11/3/2023.
//

#include "Elpida/Platform/TopologyLoader.hpp"

#include <hwloc.h>

namespace Elpida
{

	static UniquePtr<TopologyNode>
	LoadNode(hwloc_obj_t node, const Vector<CpuKind>& cpuKinds, hwloc_topology_t rootObj);

	static NodeType TranslateType(hwloc_obj_t node)
	{
		switch (node->type)
		{
		case HWLOC_OBJ_MACHINE:
			return NodeType::Machine;
		case HWLOC_OBJ_CORE:
			return NodeType::Core;
		case HWLOC_OBJ_L1CACHE:
			return NodeType::L1DCache;
		case HWLOC_OBJ_L1ICACHE:
			return NodeType::L1ICache;
		case HWLOC_OBJ_L2CACHE:
			return NodeType::L2DCache;
		case HWLOC_OBJ_L2ICACHE:
			return NodeType::L2ICache;
		case HWLOC_OBJ_L3CACHE:
			return NodeType::L3DCache;
		case HWLOC_OBJ_L3ICACHE:
			return NodeType::L3ICache;
		case HWLOC_OBJ_L4CACHE:
			return NodeType::L4Cache;
		case HWLOC_OBJ_L5CACHE:
			return NodeType::L5Cache;
		case HWLOC_OBJ_GROUP:
			return NodeType::Group;
		case HWLOC_OBJ_NUMANODE:
			return NodeType::NumaDomain;
		case HWLOC_OBJ_PACKAGE:
			return NodeType::Package;
		case HWLOC_OBJ_DIE:
			return NodeType::Die;
		case HWLOC_OBJ_PU:
			return NodeType::ProcessingUnit;
		default:
			return NodeType::Unknown;
		}
	}

	static void LoadCpuKinds(Vector<CpuKind>& cpuKinds, hwloc_topology_t topology)
	{
		auto kindsN = hwloc_cpukinds_get_nr(topology, 0);
		if (kindsN != -1)
		{
			for (auto i = 0; i < kindsN; ++i)
			{
				int efficiency = 0;
				unsigned int nrInfos = 0;
				hwloc_info_s* hwInfos;
				Map<String, String> infos;
				if (!hwloc_cpukinds_get_info(topology, i, nullptr, &efficiency, &nrInfos, &hwInfos, 0))
				{
					for (auto j = 0u; j < nrInfos; ++j)
					{
						infos.emplace(hwInfos[j].name, hwInfos[j].value);
					}
				}
				cpuKinds.emplace_back(efficiency, std::move(infos));
			}
		}
	}

	static void
	LoadDefaultProperties(TopologyNode& thisNode, hwloc_obj_t node, const Vector<CpuKind>& cpuKinds, hwloc_topology_t topology)
	{
		Optional<int> osIndex;

		auto index = node->os_index;
		if (index != HWLOC_UNKNOWN_INDEX)
		{
			thisNode.SetOsIndex(index);
		}

		{
			auto& memoryChildren = thisNode.GetMemoryChildren();
			memoryChildren.reserve(node->memory_arity);

			for (auto memChild = node->memory_first_child;
				 memChild;
				 memChild = memChild->next_sibling)
			{
				auto memChildNode = LoadNode(memChild, cpuKinds, topology);
				memChildNode->SetParent(thisNode);
				memoryChildren.push_back(std::move(memChildNode));
			}
		}

		{
			auto& children = thisNode.GetChildren();
			children.reserve(node->arity);
			for (auto child = node->first_child;
				 child;
				 child = child->next_sibling)
			{
				auto childNode = LoadNode(child, cpuKinds, topology);
				childNode->SetParent(thisNode);
				children.push_back(std::move(childNode));
			}
		}
	}

	static UniquePtr<TopologyNode>
	LoadDefault(NodeType type, hwloc_obj_t node, const Vector<CpuKind>& cpuKinds, hwloc_topology_t topology)
	{
		auto thisNode = std::make_unique<TopologyNode>(type);

		LoadDefaultProperties(*thisNode, node, cpuKinds, topology);

		return thisNode;
	}

	static UniquePtr<TopologyNode>
	LoadNumaDomain(hwloc_obj_t node, const Vector<CpuKind>& cpuKinds, hwloc_topology_t topology)
	{
		Vector<MemoryPageType> _memoryPageTypes;
		_memoryPageTypes.reserve(node->attr->numanode.page_types_len);

		for (Size i = 0; i < node->attr->numanode.page_types_len; ++i)
		{
			auto type = node->attr->numanode.page_types[i];
			_memoryPageTypes.emplace_back(type.size, type.count);
		}

		auto thisNode = std::make_unique<NumaNode>(std::move(_memoryPageTypes), node->attr->numanode.local_memory);

		LoadDefaultProperties(*thisNode, node, cpuKinds, topology);

		return thisNode;
	}

	static UniquePtr<TopologyNode>
	LoadCache(NodeType type, hwloc_obj_t node, const Vector<CpuKind>& cpuKinds, hwloc_topology_t topology)
	{
		auto size = node->attr->cache.size;
		auto associativitySets = node->attr->cache.associativity;
		auto lineSize = node->attr->cache.linesize;

		bool fullyAssociative;

		if (lineSize == 0)
		{
			lineSize = 64; // Assume the usual and hope for the best.
		}

		if (associativitySets == -1)
		{
			fullyAssociative = true;
		}

		CacheType cacheType;
		switch (node->attr->cache.type)
		{
		case HWLOC_OBJ_CACHE_UNIFIED:
			cacheType = CacheType::Unified;
			break;
		case HWLOC_OBJ_CACHE_DATA:
			cacheType = CacheType::Data;
			break;
		case HWLOC_OBJ_CACHE_INSTRUCTION:
			cacheType = CacheType::Instruction;
			break;
		}

		auto thisNode = std::make_unique<CpuCacheNode>(type, cacheType, size, lineSize, associativitySets, fullyAssociative);

		LoadDefaultProperties(*thisNode, node, cpuKinds, topology);

		return thisNode;
	}

	static UniquePtr<TopologyNode>
	LoadProcessingUnit(hwloc_obj_t node, const Vector<CpuKind>& cpuKinds, hwloc_topology_t topology)
	{
		auto kindIndex = hwloc_cpukinds_get_by_cpuset(topology, node->cpuset, 0);

		Optional<Ref<const CpuKind>> _cpuKind;

		if (kindIndex != -1)
		{
			_cpuKind = cpuKinds.at(kindIndex);
		}

		auto thisNode = std::make_unique<ProcessingUnitNode>(_cpuKind);

		LoadDefaultProperties(*thisNode, node, cpuKinds, topology);

		return thisNode;
	}

	static UniquePtr<TopologyNode> LoadNode(hwloc_obj_t node, const Vector<CpuKind>& cpuKinds, hwloc_topology_t rootObj)
	{
		auto type = TranslateType(node);
		switch (type)
		{
		case Machine:
		case Package:
		case Group:
		case Die:
		case Core:
		case Unknown:
			return LoadDefault(type, node, cpuKinds, rootObj);
		case NumaDomain:
			return LoadNumaDomain(node, cpuKinds, rootObj);
		case L1ICache:
		case L1DCache:
		case L2ICache:
		case L2DCache:
		case L3ICache:
		case L3DCache:
		case L4Cache:
		case L5Cache:
			return LoadCache(type, node, cpuKinds, rootObj);
		case ProcessingUnit:
			return LoadProcessingUnit(node, cpuKinds, rootObj);
		}
		return LoadDefault(type, node, cpuKinds, rootObj);
	}

	TopologyInfo TopologyLoader::LoadTopology() const
	{
		Vector<CpuKind> cpuKinds;
		hwloc_topology_t topology;
		try
		{
			hwloc_topology_init(&topology);
			hwloc_topology_set_all_types_filter(topology, HWLOC_TYPE_FILTER_KEEP_ALL);
			hwloc_topology_load(topology);

			LoadCpuKinds(cpuKinds, topology);

			auto root = LoadNode(hwloc_get_root_obj(topology), cpuKinds, topology);

			root->loadSiblings();

			hwloc_topology_destroy(topology);

			return {
				std::move(cpuKinds),
				std::move(root)
			};
		}
		catch (...)
		{
			hwloc_topology_destroy(topology);
			throw;
		}
	}
} // Elpida