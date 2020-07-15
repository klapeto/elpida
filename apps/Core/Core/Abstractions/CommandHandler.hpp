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
// Created by klapeto on 8/4/20.
//

#ifndef APPS_QT_CORE_ABSTRACTIONS_COMMANDHANDLER_HPP
#define APPS_QT_CORE_ABSTRACTIONS_COMMANDHANDLER_HPP

#include "CommandForwardDeclarations.hpp"

namespace Elpida
{
	class CommandHandler
	{
	public:
		virtual void handle(Command& command)
		{
		}
		virtual void handle(StartBenchmarkingCommand& command)
		{
		}
		virtual void handle(StopBenchmarkingCommand& command)
		{
		}
		virtual void handle(GetBenchmarksToRunCommand& command)
		{
		}
		virtual void handle(ShowMessageCommand& command)
		{
		}
		virtual void handle(GetTaskAffinityCommand& command)
		{
		}
		virtual void handle(SelectedBenchmarkChangedEvent& command)
		{
		}
		virtual void handle(HttpResponseEvent& command)
		{
		}
		virtual void handle(UploadResultCommand& command)
		{
		}


		virtual ~CommandHandler() = default;
	};

}

#endif //APPS_QT_CORE_ABSTRACTIONS_COMMANDHANDLER_HPP
