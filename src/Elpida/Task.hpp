//
// Created by klapeto on 27/2/2023.
//

#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <cstdlib>
#include <functional>
#include <optional>

#include "Elpida/TaskInfo.hpp"
#include "Elpida/TaskData.hpp"
#include "Elpida/Duration.hpp"
#include "Elpida/EnvironmentInfo.hpp"
#include "Elpida/TaskConfiguration.hpp"

namespace Elpida
{
	class Task
	{
	 public:

		virtual void Prepare(TaskData&& inputData) = 0;

		[[nodiscard]]
		virtual Duration Run();

		[[nodiscard]]
		virtual TaskData Finalize() = 0;

		[[nodiscard]]
		virtual TaskInfo GetInfo() const = 0;

		[[nodiscard]]
		virtual bool CanBeMultiThreaded() const = 0;

		[[nodiscard]]
		std::unique_ptr<Task> Duplicate() const;

		void SetEnvironmentInfo(const EnvironmentInfo& environmentInfo);

		Task() = default;
		virtual ~Task() = default;
	 protected:
		std::optional<std::reference_wrapper<const EnvironmentInfo>> _environmentInfo;

		virtual void DoRun() = 0;

		[[nodiscard]]
		virtual std::unique_ptr<Task> DoDuplicate() const = 0;
	};

} // Elpida

#endif //_TASK_HPP_
