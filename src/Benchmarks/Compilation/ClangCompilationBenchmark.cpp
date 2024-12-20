//
// Created by klapeto on 15/12/2024.
//

#include "ClangCompilationBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/GenerateRandomDataTask.hpp"
#include "CommonTasks/GenerateLoremIpsum.hpp"
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
		name = "C++ compilation";
		description = "Compiles C++ code with Clang";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(GenerateCppCodeTask().GetInfo());
		taskInfos.push_back(CompileWithClangTask().GetInfo());
	}
} // Elpida