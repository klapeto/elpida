/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 10/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP

#include <vector>

namespace Elpida
{
	class Task;
	class TaskAffinity;
	class DataSpecification;
	class RawData;

	class DataAdapter
	{
	public:
		static void adaptAndForwardTaskData(Task* previous, Task* next);

		[[nodiscard]] static std::vector<RawData*> breakIntoChunks(const RawData& input,
			const TaskAffinity& affinity,
			const DataSpecification& targetDataSpecification);

		[[nodiscard]] static RawData* mergeIntoSingleChunk(const std::vector<const RawData*>& inputData);

		virtual ~DataAdapter() = default;
	protected:
		static size_t getAccumulatedSizeOfChunks(const std::vector<const RawData*>& outputChunks);
		static std::vector<RawData*> breakIntoChunksImpl(const std::vector<const RawData*>& input,
			const std::vector<int>& processorsOsIndices,
			size_t chunksDivisibleBy);
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP
