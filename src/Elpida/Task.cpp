//
// Created by klapeto on 27/2/2023.
//

#include "Task.hpp"

namespace Elpida
{

	Duration Task::Run()
	{
		auto a = Timer::now();

		DoRun();

		auto b = Timer::now();

		return b - a - _overheadsInfo.GetNowOverhead();
	}

	std::unique_ptr<Task> Task::Duplicate() const
	{
		auto task = DoDuplicate();
		task->SetOverheads(_overheadsInfo);
		return std::move(task);
	}

	void Task::SetOverheads(const OverheadsInfo& overheadsInfo)
	{
		_overheadsInfo = overheadsInfo;
	}
} // Elpida