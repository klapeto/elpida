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
		TaskInfo GetInfo() const
		{
			auto info = DoGetInfo();
			info._isMeasured = _measured;
			return std::move(info);
		}

		[[nodiscard]]
		virtual bool CanBeMultiThreaded() const = 0;

		[[nodiscard]]
		virtual Size GetProcessedDataSize() const = 0;

		[[nodiscard]]
		UniquePtr<Task> Duplicate() const;

		Task& SetMeasured(bool measured)
		{
			_measured = measured;
			return *this;
		}

		[[nodiscard]]
		bool ShouldBeMeasured() const
		{
			return _measured;
		}

		void SetEnvironmentInfo(const EnvironmentInfo& environmentInfo);

		Task() = default;
		virtual ~Task() = default;
	protected:
		Optional<Ref<const EnvironmentInfo>> _environmentInfo;
		bool _measured = true;

		[[nodiscard]]
		virtual TaskInfo DoGetInfo() const = 0;

		virtual void DoRun() = 0;

		[[nodiscard]]
		virtual Duration PostProcessDuration(const Duration& duration) const;

		[[nodiscard]]
		virtual UniquePtr<Task> DoDuplicate() const = 0;

		template<typename T, typename ... TArgs>
		UniquePtr<T> CreateData(TArgs&& ... args)
		{
			return std::make_unique<T>(std::forward<TArgs>(args)...);
		}
	};

} // Elpida

#endif //ELPIDA_TASK_HPP_
