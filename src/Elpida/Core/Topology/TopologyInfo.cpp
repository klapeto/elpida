//
// Created by klapeto on 2/3/2023.
//

#include "Elpida/Core/Topology/TopologyInfo.hpp"
#include "Elpida/Core/Map.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <threads.h>
#include <hwloc.h>

namespace Elpida
{
	const TopologyNode& TopologyInfo::GetRoot() const
	{
		return *_root;
	}

	const Vector<CpuKind>& TopologyInfo::GetCpuKinds() const
	{
		return _cpuKinds;
	}

	const Vector<Ref<const CpuCacheNode>>& TopologyInfo::GetAllCaches() const
	{
		return _allCaches;
	}

	const Vector<Ref<const NumaNode>>& TopologyInfo::GetAllNumaNodes() const
	{
		return _allNumaNodes;
	}

	const Vector<Ref<const ProcessingUnitNode>>& TopologyInfo::GetAllProcessingUnits() const
	{
		return _allProcessingUnits;
	}

	const Vector<Ref<const TopologyNode>>& TopologyInfo::GetAllCores() const
	{
		return _allCores;
	}

	TopologyInfo::TopologyInfo(UniquePtr<TopologyNode> root,
			Vector<CpuKind>&& cpuKinds,
			Vector<Ref<const CpuCacheNode>>&& allCaches,
			Vector<Ref<const NumaNode>>&& allNumaNodes,
			Vector<Ref<const TopologyNode>>&& allCores,
			Vector<Ref<const ProcessingUnitNode>> allProcessingUnits,
			void* topologyObj)
			:_cpuKinds(std::move(cpuKinds)),
			 _allCaches(std::move(allCaches)),
			 _allNumaNodes(std::move(allNumaNodes)),
			 _allCores(std::move(allCores)),
			 _allProcessingUnits(std::move(allProcessingUnits)),
			 _root(std::move(root)),
			 _topologyObj(topologyObj)
	{

	}

	TopologyInfo::~TopologyInfo()
	{
		if (_topologyObj != nullptr)
		{
			hwloc_topology_destroy((hwloc_topology_t)_topologyObj);
		}
	}

	TopologyInfo::TopologyInfo(TopologyInfo&& other) noexcept
			:_cpuKinds(std::move(other._cpuKinds)),
			 _allCaches(std::move(other._allCaches)),
			 _allNumaNodes(std::move(other._allNumaNodes)),
			 _allCores(std::move(other._allCores)),
			 _allProcessingUnits(std::move(other._allProcessingUnits)),
			 _root(std::move(other._root)),
			 _topologyObj(other._topologyObj)
	{
		other._topologyObj = nullptr;
	}

	TopologyInfo& TopologyInfo::operator=(TopologyInfo&& other) noexcept
	{
		_cpuKinds = std::move(other._cpuKinds);
		_allCaches = std::move(other._allCaches);
		_allNumaNodes = std::move(other._allNumaNodes);
		_allCores = std::move(other._allCores);
		_allProcessingUnits = std::move(other._allProcessingUnits);
		_root = std::move(other._root);
		_topologyObj = other._topologyObj;
		other._topologyObj = nullptr;
		return *this;
	}

	void TopologyInfo::PinThreadToProcessor(unsigned int processorId) const
	{
		auto set = hwloc_bitmap_alloc();
		if (set == nullptr)
		{
			throw ElpidaException("Failed to allocate for thread hwloc bitmap");
		}
		try
		{
			if (hwloc_bitmap_set(set, processorId) == -1)
			{
				throw ElpidaException("Failed to set thread bitmap: ", strerror(errno));
			}
			if (hwloc_set_thread_cpubind((hwloc_topology_t)_topologyObj, thrd_current(), set, 0) == -1)
			{
				throw ElpidaException("Failed to set thread bind: ", strerror(errno));
			}
		}
		catch (...)
		{
			hwloc_bitmap_free(set);
			throw;
		}
		hwloc_bitmap_free(set);
	}

	void TopologyInfo::PinProcessToProcessors(const std::vector<Ref<const ProcessingUnitNode>>& processors) const
	{
		auto set = hwloc_bitmap_alloc();

		if (set == nullptr)
		{
			throw ElpidaException("Failed to allocate for process hwloc bitmap");
		}

		try
		{
			for (auto& processor : processors)
			{
				if (hwloc_bitmap_set(set, processor.get().GetOsIndex().value()) == -1)
				{
					throw ElpidaException("Failed to set process bitmap: ", strerror(errno));
				}
			}
			if (hwloc_set_cpubind((hwloc_topology_t)_topologyObj, set, 0) == -1)
			{
				throw ElpidaException("Failed to set process bind: ", strerror(errno));
			}
		}
		catch (...)
		{
			hwloc_bitmap_free(set);
			throw;
		}

		hwloc_bitmap_free(set);
	}

} // Elpida