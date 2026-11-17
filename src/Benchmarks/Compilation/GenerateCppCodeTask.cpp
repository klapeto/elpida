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
// Created by klapeto on 20/12/2024.
//

#include "GenerateCppCodeTask.hpp"
#include <cstring>

namespace Elpida::Benchmarks::Compilation
{
	// Generated at build time from Assets/CompilationBenchmarkSource.cpp (see EmbedFile.cmake). The array ends with a
	// NUL that is not counted in the size: the compiler reads the source as a NUL-terminated buffer.
	extern const unsigned char CompilationBenchmarkSource[];
	extern const std::size_t CompilationBenchmarkSource_SIZE;

	void GenerateCppCodeTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_outputData = std::move(inputData);
		_outputData->Allocate(CompilationBenchmarkSource_SIZE + 1);
	}

	SharedPtr<AbstractTaskData> GenerateCppCodeTask::Finalize()
	{
		return std::move(_outputData);
	}

	Size GenerateCppCodeTask::GetProcessedDataSize() const
	{
		return _outputData->GetSize();
	}

	TaskInfo GenerateCppCodeTask::DoGetInfo() const
	{
		return {
				"C++ source generation",
				"Generates C++ source code",
				"Chars",
				"The byte generation throughput",
				ResultType::Throughput
		};
	}

	void GenerateCppCodeTask::DoRun()
	{
		std::memcpy(_outputData->GetData(), CompilationBenchmarkSource, CompilationBenchmarkSource_SIZE + 1);
	}

	UniquePtr<Task> GenerateCppCodeTask::DoDuplicate() const
	{
		return std::make_unique<GenerateCppCodeTask>();
	}
} // Elpida