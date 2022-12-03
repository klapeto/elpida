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

#ifndef APPS_QT_CORE_ABSTRACTIONS_MEDIATOR_HPP
#define APPS_QT_CORE_ABSTRACTIONS_MEDIATOR_HPP

namespace Elpida
{
	class Command;

	class Mediator
	{
	public:
		virtual void execute(Command& command) = 0;
		virtual void execute(Command&& command) = 0;

		virtual void waitToContinue() = 0;
		virtual void signalToContinue() = 0;
		virtual void setToWait() = 0;

		virtual ~Mediator() = default;
	};
}

#endif //APPS_QT_CORE_ABSTRACTIONS_MEDIATOR_HPP
