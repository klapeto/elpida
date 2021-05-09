/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * SystemTopology.hpp
 *
 *  Created on: 12 Ιαν 2019
 *      Author: klapeto
 */

#ifndef ELPIDA_TOPOLOGY_SYSTEMTOPOLOGY_HPP_
#define ELPIDA_TOPOLOGY_SYSTEMTOPOLOGY_HPP_

#include <vector>
#include <memory>
#include "CpuKind.hpp"
#include "ProcessorNode.hpp"


namespace Elpida
{
	class SystemTopology final
	{
	public:
		enum class ProcessPriority
		{
			Normal, High,
		};

		[[nodiscard]]
		const std::vector<const ProcessorNode*>& getAllProcessors() const
		{
			return _allProcessors;
		}

		[[nodiscard]]
		std::size_t getDepth() const
		{
			return _depth;
		}

		[[nodiscard]] const ProcessorNode& getRoot() const
		{
			return *_root;
		}

		[[nodiscard]]
		std::size_t getTotalLogicalCores() const
		{
			return _totalLogicalCores;
		}

		[[nodiscard]]
		std::size_t getTotalPhysicalCores() const
		{
			return _totalPhysicalCores;
		}

		[[nodiscard]]
		std::size_t getTotalNumaNodes() const
		{
			return _totalNumaNodes;
		}

		[[nodiscard]]
		std::size_t getTotalPackages() const
		{
			return _totalPackages;
		}

		[[nodiscard]]
		std::size_t getTotalMachines() const
		{
			return _totalMachines;
		}

		static void setProcessPriority(ProcessPriority priority);

		SystemTopology();
		~SystemTopology() = default;
	private:
		std::vector<const ProcessorNode*> _allProcessors;
		std::vector<CpuKind> _cpuKinds;
		std::unique_ptr<ProcessorNode> _root;

		std::size_t _depth;
		std::size_t _totalLogicalCores;
		std::size_t _totalPhysicalCores;
		std::size_t _totalNumaNodes;
		std::size_t _totalPackages;
		std::size_t _totalMachines;

		void reload();
		void accumulateCores(const ProcessorNode& node);

		void processChildNode(const ProcessorNode& node);
	};

} /* namespace Elpida */

#endif /* ELPIDA_TOPOLOGY_SYSTEMTOPOLOGY_HPP_ */
