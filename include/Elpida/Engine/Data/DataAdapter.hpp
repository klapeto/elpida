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
#include <memory>
#include "Elpida/Engine/Data/RawTaskData.hpp"

namespace Elpida
{
	class TaskAffinity;
	class ProcessorNode;

	class DataAdapter final
	{
	public:
		[[nodiscard]] static std::vector<std::unique_ptr<RawTaskData>> breakIntoChunks(
			const std::vector<const RawTaskData*>& inputData,
			const TaskAffinity& affinity,
			size_t divisibleBy);

		[[nodiscard]] static std::unique_ptr<RawTaskData> mergeIntoSingleChunk(
			const std::vector<const RawTaskData*>& inputData,
			const ProcessorNode& processor);

	protected:
		static size_t getAccumulatedSizeOfChunks(const std::vector<const RawTaskData*>& outputChunks);

		static std::vector<std::unique_ptr<RawTaskData>> breakIntoChunksImpl(
			const std::vector<const RawTaskData*>& input,
			const std::vector<const ProcessorNode*>& processors,
			size_t chunksDivisibleBy);
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_TASK_DATA_DATAADAPTER_HPP
