//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_GETSELECTEDTASKBATCHESCOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_GETSELECTEDTASKBATCHESCOMMAND_HPP

#include <vector>
#include "Core/Abstractions/TypedCommand.hpp"

namespace Elpida
{
	class TaskBatch;
	class GetSelectedTaskBatchesCommand final : public TypedCommand<GetSelectedTaskBatchesCommand>
	{
	public:
		void addTaskBatch(const TaskBatch& taskBatch)
		{
			_taskBatches.push_back(&taskBatch);
		}

		[[nodiscard]] const std::vector<const TaskBatch*>& getTaskBatches() const
		{
			return _taskBatches;
		}

		GetSelectedTaskBatchesCommand() = default;
		~GetSelectedTaskBatchesCommand() override = default;
	private:
		std::vector<const TaskBatch*> _taskBatches;
	};

}


#endif //APPS_QT_CORE_COMMANDS_GETSELECTEDTASKBATCHESCOMMAND_HPP
