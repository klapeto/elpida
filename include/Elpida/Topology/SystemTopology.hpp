/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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

#include <cstddef>
#include <vector>

namespace Elpida
{

	class ProcessorNode;

	class SystemTopology final
	{
		public:
			const std::vector<const ProcessorNode*>& getAllProcessors() const
			{
				return _allProcessors;
			}

			std::size_t getDepth() const
			{
				return _depth;
			}

			const ProcessorNode* getRoot() const
			{
				return _root;
			}

			std::size_t getTotalLogicalCores() const
			{
				return _totalLogicalCores;
			}

			std::size_t getTotalPhysicalCores() const
			{
				return _totalPhysicalCores;
			}

			static int getNumaNodeOfProcessor(int processorId);

			static const SystemTopology& getTopology()
			{
				static SystemTopology topo;
				return topo;
			}

			~SystemTopology();

		private:
			std::vector<const ProcessorNode*> _allProcessors;
			ProcessorNode* _root;

			std::size_t _depth;
			std::size_t _totalLogicalCores;
			std::size_t _totalPhysicalCores;

			SystemTopology();
			void reload();
			void accumulateCores(const ProcessorNode& node);
	};

} /* namespace Elpida */

#endif /* ELPIDA_TOPOLOGY_SYSTEMTOPOLOGY_HPP_ */
