/*
 * TaskTopology.hpp
 *
 *  Created on: 16 Νοε 2019
 *      Author: klapeto
 */

#ifndef ELPIDA_TASKAFFINITY_HPP_
#define ELPIDA_TASKAFFINITY_HPP_

#include <vector>

namespace Elpida
{
	class ProcessorNode;

	class TaskAffinity final
	{
		public:

			bool isSet() const
			{
				return _nodes.size() > 0;
			}

			const std::vector<const ProcessorNode*>& getProcessorNodes() const
			{
				return _nodes;
			}

			TaskAffinity()
			{
			}

			TaskAffinity(std::initializer_list<int> processors);

			TaskAffinity(const std::vector<const ProcessorNode*>& nodesToUse)
					: _nodes(nodesToUse)
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
