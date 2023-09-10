//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Core/Task.hpp"

#include "Elpida/Core/EnvironmentInfo.hpp"

namespace Elpida
{

	Duration Task::Run()
	{
		auto a = Timer::now();

		DoRun();

		auto b = Timer::now();

		return b - a - _environmentInfo->get().GetOverheadsInfo().GetNowOverhead();
	}

	UniquePtr<Task> Task::Duplicate() const
	{
		auto task = DoDuplicate();
		task->SetEnvironmentInfo(_environmentInfo->get());
		return task;
	}

	void Task::SetEnvironmentInfo(const EnvironmentInfo& environmentInfo)
	{
		_environmentInfo = environmentInfo;
	}
} // Elpida