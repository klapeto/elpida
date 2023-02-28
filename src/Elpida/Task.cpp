//
// Created by klapeto on 27/2/2023.
//

#include "Task.hpp"

namespace Elpida
{

	Duration Task::Run()
	{
		auto a = std::chrono::high_resolution_clock::now();

		DoRun();

		auto b = std::chrono::high_resolution_clock::now();

		return b - a;
	}

	std::unique_ptr<Task> Task::Duplicate() const
	{
		auto task = DoDuplicate();
		task->SetOverheads(_overheadsInfo);
		return std::move(task);
	}
} // Elpida