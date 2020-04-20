//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_GETSELECTEDTASKBATCHESCOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_GETSELECTEDTASKBATCHESCOMMAND_HPP

#include <vector>
#include "Core/Abstractions/TypedCommand.hpp"
#include <Elpida/Engine/Runner/BenchmarkRunRequest.hpp>

namespace Elpida
{
	class GetSelectedTaskBatchesCommand final : public TypedCommand<GetSelectedTaskBatchesCommand>
	{
	public:
		void addTaskBatch(const BenchmarkRunRequest& taskBatch)
		{
			_taskBatches.push_back(taskBatch);
		}

		[[nodiscard]] const std::vector<BenchmarkRunRequest>& getTaskBatches() const
		{
			return _taskBatches;
		}

		GetSelectedTaskBatchesCommand() = default;
		~GetSelectedTaskBatchesCommand() override = default;
	private:
		std::vector<BenchmarkRunRequest> _taskBatches;
	};

}


#endif //APPS_QT_CORE_COMMANDS_GETSELECTEDTASKBATCHESCOMMAND_HPP
