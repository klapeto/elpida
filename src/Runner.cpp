/**************************************************************************
*   elpida - CPU benchmark tool
*   
*   Copyright (C) 2018  Ioannis Panagiotopoulos
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

/*
 * Runner.cpp
 *
 *  Created on: 8 Μαρ 2018
 *      Author: klapeto
 */

#include "Config.hpp"
#include "Runner.hpp"
#include <iostream>

#if _elpida_linux
#include <sys/resource.h>
#elif _elpida_windows
#include <Windows.h>
#endif

namespace Elpida
{

	Runner::Runner()
	{
	}

	Runner::~Runner()
	{
	}

	void Runner::executeTasks() const
	{

	}

	void Runner::setProcessPriority(ProcessPriority priority)
	{
#if _elpida_linux
		switch (priority)
		{
			case ProcessPriority::High:
				setpriority(PRIO_PROCESS, 0, PRIO_MIN);
				break;
			default:
				setpriority(PRIO_PROCESS, 0, 0);
				break;
		}
#elif _elpida_windows
		if(!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
		{
			std::cout << "Warning! Failed to set process priority: " << GetLastError() << std::endl;
		}
#endif
	}

}/* namespace Elpida */
