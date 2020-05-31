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
 * TaskTopology.hpp
 *
 *  Created on: 16 Νοε 2019
 *      Author: klapeto
 */

#ifndef ELPIDA_ENGINE_TASK_TASKAFFINITY_HPP_
#define ELPIDA_ENGINE_TASK_TASKAFFINITY_HPP_

#include <vector>
#include <forward_list>

namespace Elpida
{
	class ProcessorNode;

	class TaskAffinity final
	{
	public:

		[[nodiscard]] const std::vector<const ProcessorNode*>& getProcessorNodes() const
		{
			return _nodes;
		}

		[[nodiscard]] std::vector<const ProcessorNode*> getProcessors(size_t processorsToUse) const
		{
			std::vector<const ProcessorNode*> returnProcessors;
			auto processors = processorsToUse <= _nodes.size() ? processorsToUse : _nodes.size();
			for (size_t i = 0; i < processors; ++i)
			{
				returnProcessors.push_back(_nodes[i]);
			}
			return returnProcessors;
		}

		TaskAffinity() = default;

		template<typename T>
		explicit TaskAffinity(const T& nodesToUse)
			: _nodes(nodesToUse.begin(), nodesToUse.end())
		{

		}

		TaskAffinity(const TaskAffinity& other) = default;
		TaskAffinity(TaskAffinity&& other) noexcept
			: _nodes(std::move(other._nodes))
		{

		}

		TaskAffinity& operator=(const TaskAffinity& other) = default;
		TaskAffinity& operator=(TaskAffinity&& other) noexcept
		{
			_nodes = std::move(other._nodes);
			return *this;
		}

		~TaskAffinity() = default;
	private:
		std::vector<const ProcessorNode*> _nodes;
	};

} /* namespace Elpida */

#endif /* ELPIDA_ENGINE_TASK_TASKAFFINITY_HPP_ */
