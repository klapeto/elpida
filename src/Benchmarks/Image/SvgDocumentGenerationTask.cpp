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
// Created by klapeto on 7/5/2024.
//

#include "SvgDocumentGenerationTask.hpp"
#include "Elpida/Core/RawTaskData.hpp"
#include <cstring>

namespace Elpida::Benchmarks::Image
{
	// Generated at build time from Assets/SvgBenchmarkDocument.svg (see EmbedFile.cmake)
	extern const unsigned char SvgBenchmarkDocument[];
	extern const std::size_t SvgBenchmarkDocument_SIZE;

	void SvgDocumentGenerationTask::Prepare(SharedPtr<AbstractTaskData> inputData)
	{
		_outputData = std::make_unique<RawTaskData>(inputData->GetAllocator());
		_outputData->Allocate(SvgBenchmarkDocument_SIZE);
	}

	SharedPtr<AbstractTaskData> SvgDocumentGenerationTask::Finalize()
	{
		return std::move(_outputData);
	}

	Size SvgDocumentGenerationTask::GetProcessedDataSize() const
	{
		return _outputData->GetSize();
	}

	void SvgDocumentGenerationTask::DoRun()
	{
		std::memcpy(_outputData->GetData(), SvgBenchmarkDocument, SvgBenchmarkDocument_SIZE);
	}

	UniquePtr<Task> SvgDocumentGenerationTask::DoDuplicate() const
	{
		return std::make_unique<SvgDocumentGenerationTask>();
	}

	TaskInfo SvgDocumentGenerationTask::DoGetInfo() const
	{
		return {
				"Svg Document Generation",
				"Copies the built-in Svg document",
				"B",
				"How many bytes are copied",
				ResultType::Throughput
		};
	}
} // Elpida
