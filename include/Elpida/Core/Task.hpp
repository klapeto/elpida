//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_TASK_HPP_
#define ELPIDA_TASK_HPP_

#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/Optional.hpp"
#include "Elpida/Core/TaskInfo.hpp"
#include "Elpida/Core/RawTaskData.hpp"
#include "Elpida/Core/AbstractTaskData.hpp"
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/EnvironmentInfo.hpp"
#include "Elpida/Core/TaskConfiguration.hpp"

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

#endif //ELPIDA_TASK_HPP_
