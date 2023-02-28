//
// Created by klapeto on 27/2/2023.
//

#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <cstdlib>

#include "Elpida/TaskInfo.hpp"
#include "Elpida/TaskData.hpp"
#include "Elpida/Duration.hpp"
#include "Elpida/OverheadsInfo.hpp"

namespace Elpida
{
	class Task
	{
	 public:
		virtual void Prepare(TaskData&& inputData) = 0;
		virtual Duration Run();
		virtual TaskData Finalize() = 0;

		virtual TaskInfo GetInfo() const = 0;
		virtual bool CanBeMultiThreaded() const = 0;
		std::unique_ptr<Task> Duplicate() const;

		void SetOverheads(const OverheadsInfo& overheadsInfo)
		{
			_overheadsInfo = overheadsInfo;
		}

		Task() = default;
		virtual ~Task() = default;
	 protected:
		virtual void DoRun() = 0;
		virtual std::unique_ptr<Task> DoDuplicate() const=0;
		OverheadsInfo _overheadsInfo;
	};

} // Elpida

#endif //_TASK_HPP_
