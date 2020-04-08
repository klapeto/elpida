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
 * TaskTopology.hpp
 *
 *  Created on: 16 Νοε 2019
 *      Author: klapeto
 */

#ifndef ELPIDA_TASKAFFINITY_HPP_
#define ELPIDA_TASKAFFINITY_HPP_

#include <vector>
#include <forward_list>

namespace Elpida
{
	class ProcessorNode;

	class TaskAffinity final
	{
	public:

		const std::vector<const ProcessorNode*>& getProcessorNodes() const
		{
			return _nodes;
		}

		TaskAffinity()
		{
		}

		TaskAffinity(std::initializer_list<ProcessorNode*> processors);

		template<typename T>
		TaskAffinity(const T& nodesToUse)
			: _nodes(nodesToUse.begin(), nodesToUse.end())
		{

		}

		TaskAffinity(std::vector<const ProcessorNode*>&& nodesToUse)
			: _nodes(std::move(nodesToUse))
		{

		}

		TaskAffinity(const TaskAffinity& other)
			: _nodes(other._nodes)
		{

		}

		TaskAffinity(TaskAffinity&& other)
			: _nodes(std::move(other._nodes))
		{

		}

		TaskAffinity& operator=(const TaskAffinity& other)
		{
			_nodes = other._nodes;
			return *this;
		}

		TaskAffinity& operator=(TaskAffinity&& other)
		{
			_nodes = std::move(other._nodes);
			return *this;
		}

		~TaskAffinity()
		{

		}

	private:
		std::vector<const ProcessorNode*> _nodes;
	};

} /* namespace Elpida */

#endif /* ELPIDA_TASKAFFINITY_HPP_ */
