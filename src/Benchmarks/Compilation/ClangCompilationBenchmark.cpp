/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 15/12/2024.
//

#include "ClangCompilationBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/GenerateRandomDataTask.hpp"
#include "GenerateCppCodeTask.hpp"
#include "CompileWithClangTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> ClangCompilationBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	std::vector<std::unique_ptr<Task>> ClangCompilationBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		Vector<UniquePtr<Task>> tasks;

		tasks.push_back(CreateTask<GenerateCppCodeTask>());
		tasks.push_back(CreateTask<CompileWithClangTask>());

		return tasks;
	}

	void ClangCompilationBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "C++ Compilation";
		description = "Compiles C++ code with Clang";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(GenerateCppCodeTask().GetInfo());
		taskInfos.push_back(CompileWithClangTask().GetInfo());
	}
} // Elpida