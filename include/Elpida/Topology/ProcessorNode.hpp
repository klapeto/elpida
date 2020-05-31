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

namespace Elpida
{

	class ProcessorNode final
	{
	public:
		enum class Type
		{
			Machine,
			Package,
			NumaNode,
			Group,
			Core,
			L1ICache,
			L1DCache,
			L2ICache,
			L2DCache,
			L3ICache,
			L3DCache,
			L4Cache,
			L5Cache,
			ExecutionUnit,
			Unknown
		};

		[[nodiscard]] unsigned int getOsIndex() const
		{
			return _osIndex;
		}

		[[nodiscard]] const std::vector<ProcessorNode>& getChildren() const
		{
			return _children;
		}

		[[nodiscard]] const std::vector<ProcessorNode*>& getSiblings() const
		{
			return _siblings;
		}

		[[nodiscard]] const std::vector<ProcessorNode>& getMemoryChildren() const
		{
			return _memoryChildren;
		}

		[[nodiscard]] const std::string& getName() const
		{
			return _name;
		}

		[[nodiscard]] Type getType() const
		{
			return _type;
		}

		[[nodiscard]] std::size_t getValue() const
		{
			return _value;
		}

		~ProcessorNode() = default;
	private:
		ProcessorNode(ProcessorNode* parent, void* node);    // Evil casting to avoid header inclusion

		void addSibling(ProcessorNode& node)
		{
			_siblings.push_back(&node);
		}

		std::string _name;
		Type _type;
		std::size_t _value;
		unsigned int _osIndex;
		ProcessorNode* _parent;

		std::vector<ProcessorNode> _children;
		std::vector<ProcessorNode> _memoryChildren;
		std::vector<ProcessorNode*> _siblings;
		void loadMachine(void* node);
		void loadPackage(void* node);
		void loadNumaNode(void* node);
		void loadGroup(void* node);
		void loadCore(void* node);
		void loadCache(void* node);
		void loadExecutionUnit(void* node);

		void loadChildren(void* node);
		void loadSiblings();
		friend class SystemTopology;
	};

} /* namespace Elpida */

#endif /* ELPIDA_TOPOLOGY_PROCESSORNODE_HPP_ */
