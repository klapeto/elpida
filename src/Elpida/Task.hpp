//
// Created by klapeto on 27/2/2023.
//

#ifndef _TASK_HPP_
#define _TASK_HPP_

#include "Elpida/Size.hpp"
#include "Elpida/Ref.hpp"
#include "Elpida/Optional.hpp"
#include "Elpida/TaskInfo.hpp"
#include "Elpida/RawTaskData.hpp"
#include "Elpida/AbstractTaskData.hpp"
#include "Elpida/Duration.hpp"
#include "Elpida/EnvironmentInfo.hpp"
#include "Elpida/TaskConfiguration.hpp"

namespace Elpida
{
	class Task
	{
	 public:
		virtual void Prepare(UniquePtr<AbstractTaskData> inputData) = 0;

		[[nodiscard]]
		virtual Duration Run();

		[[nodiscard]]
		virtual UniquePtr<AbstractTaskData> Finalize() = 0;

		[[nodiscard]]
		virtual TaskInfo GetInfo() const = 0;

		[[nodiscard]]
		virtual bool CanBeMultiThreaded() const = 0;

		[[nodiscard]]
		UniquePtr<Task> Duplicate() const;

		void SetEnvironmentInfo(const EnvironmentInfo& environmentInfo);

		Task() = default;
		virtual ~Task() = default;
	 protected:
		Optional<Ref<const EnvironmentInfo>> _environmentInfo;

		virtual void DoRun() = 0;

		[[nodiscard]]
		virtual UniquePtr<Task> DoDuplicate() const = 0;
	};

} // Elpida

#endif //_TASK_HPP_
