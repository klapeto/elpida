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
 * Task.hpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_TASK_HPP_
#define ELPIDA_TASK_HPP_

#include <string>
#include <vector>

#include "Elpida/TaskAffinity.hpp"

namespace Elpida
{
	class TaskMetrics;
	class TaskRunResult;

	class Task
	{
		public:

			bool isToBeMeasured() const
			{
				return _toBeMeasured;
			}

			void setToBeMeasured(bool toBeMeasured)
			{
				_toBeMeasured = toBeMeasured;
			}

			inline const std::string& getName() const
			{
				return _name;
			}

			const std::vector<const TaskRunResult*>& getLastRunResults() const
			{
				return _lastRunResults;
			}

			void clearResults()
			{
				_lastRunResults.clear();
			}

			virtual void run() = 0;
			virtual void calculateResults(const TaskMetrics& metrics) = 0;

			virtual void applyAffinity();

			virtual void prepare()
			{

			}

			virtual void finalize()
			{

			}

			void setAffinity(const TaskAffinity& affinity)
			{
				_affinity = affinity;
			}

			void setAffinity(TaskAffinity&& affinity)
			{
				_affinity = std::move(affinity);
			}

			Task(const std::string& name, bool toBeMeasured = true)
					: _name(name), _toBeMeasured(toBeMeasured)
			{

			}

			virtual ~Task()
			{

			}

			Task(Task&&) = default;
			Task(const Task&) = default;
			Task& operator=(Task&&) = default;
			Task& operator=(const Task&) = default;
		protected:
			TaskAffinity _affinity;

			void addResult(const TaskRunResult& result)
			{
				_lastRunResults.push_back(&result);
			}
		private:
			std::vector<const TaskRunResult*> _lastRunResults;
			std::string _name;
			bool _toBeMeasured;
	};

} /* namespace Elpida */

#endif /* ELPIDA_TASK_HPP_ */
