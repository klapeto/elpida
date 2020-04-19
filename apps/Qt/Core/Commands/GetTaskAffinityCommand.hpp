//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_GETTASKAFFINITYCOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_GETTASKAFFINITYCOMMAND_HPP

#include <Elpida/TaskAffinity.hpp>
#include "Core/Abstractions/TypedCommand.hpp"

namespace Elpida  {
	class GetTaskAffinityCommand: public TypedCommand<GetTaskAffinityCommand>
	{
	public:
		[[nodiscard]] const TaskAffinity& getAffinity() const
		{
			return _affinity;
		}

		void setAffinity(const TaskAffinity& affinity)
		{
			_affinity = affinity;
		}

		GetTaskAffinityCommand() = default;
	private:
		TaskAffinity _affinity;
	};
}



#endif //APPS_QT_CORE_COMMANDS_GETTASKAFFINITYCOMMAND_HPP
