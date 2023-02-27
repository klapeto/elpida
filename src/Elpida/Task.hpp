//
// Created by klapeto on 27/2/2023.
//

#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <cstdlib>
#include "Elpida/TaskInfo.hpp"
#include "Elpida/Duration.hpp"

namespace Elpida
{
	class TaskData;

	class Task
	{
	 public:
		virtual void Prepare(TaskData& inputData) = 0;
		virtual Duration Run();
		virtual void Finalize(TaskData& outputData) = 0;

		virtual TaskInfo GetInfo() const = 0;

		Task() = default;
		virtual ~Task() = default;
	 protected:
		virtual void DoRun() = 0;
	};

} // Elpida

#endif //_TASK_HPP_
