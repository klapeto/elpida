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
 * ProcessorNode.hpp
 *
 *  Created on: 12 Ιαν 2019
 *      Author: klapeto
 */

#ifndef ELPIDA_TOPOLOGY_PROCESSORNODE_HPP_
#define ELPIDA_TOPOLOGY_PROCESSORNODE_HPP_

#include <string>
#include <vector>

#include "CpuKind.hpp"
#include "Elpida/Utilities/OptionalReference.hpp"
#include "ProcessorNodeType.hpp"

namespace Elpida
{

	class ProcessorNode final
	{
	public:
		[[nodiscard]]
		unsigned int getOsIndex() const
		{
			return _osIndex;
		}

		[[nodiscard]]
		const std::vector<ProcessorNode>& getChildren() const
		{
			return _children;
		}

		[[nodiscard]]
		const std::vector<ProcessorNode*>& getSiblings() const
		{
			return _siblings;
		}

		[[nodiscard]]
		bool isOsIndexValid() const;

		[[nodiscard]]
		const std::vector<ProcessorNode>& getMemoryChildren() const
		{
			return _memoryChildren;
		}

		[[nodiscard]]
		const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]]
		ProcessorNodeType getType() const
		{
			return _type;
		}

		[[nodiscard]]
		std::size_t getValue() const
		{
			return _value;
		}

		[[nodiscard]]
		OptionalReference<const CpuKind> getCpuKind() const
		{
			return _cpuKind;
		}

		[[nodiscard]]
		OptionalReference<ProcessorNode> getParent() const
		{
			return _parent;
		}

		[[nodiscard]]
		int getNumaNodeId() const;

		~ProcessorNode() = default;
	private:
		ProcessorNode(ProcessorNode* parent, const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node);    // Evil casting to avoid header inclusion

		void addSibling(ProcessorNode& node)
		{
			_siblings.push_back(&node);
		}

		std::string _name;
		std::size_t _value;
		OptionalReference<ProcessorNode> _parent;
		OptionalReference<const CpuKind> _cpuKind;
		ProcessorNodeType _type;
		unsigned int _osIndex;

		std::vector<ProcessorNode> _children;
		std::vector<ProcessorNode> _memoryChildren;
		std::vector<ProcessorNode*> _siblings;
		void loadMachine(void* node);
		void loadPackage(void* node);
		void loadDie(void* node);
		void loadNumaNode(void* node);
		void loadGroup(void* node);
		void loadCore(void* node);
		void loadCache(void* node);
		void loadExecutionUnit(void* node);

		void loadChildren(const std::vector<CpuKind>& cpuKinds, void* rootObj, void* node);
		void loadSiblings();
		void loadParents(ProcessorNode* parent);
		friend class SystemTopology;
	};

} /* namespace Elpida */

#endif /* ELPIDA_TOPOLOGY_PROCESSORNODE_HPP_ */
