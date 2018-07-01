/**************************************************************************
*   Elpida -  Benchmark library
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
 * Cpuid.cpp
 *
 *  Created on: 1 Ιουλ 2018
 *      Author: klapeto
 */

#include "Cpuid.hpp"
#include "CpuInfo.hpp"
#include "BadCommandException.hpp"
#include <iostream>

namespace Elpida
{
	namespace CLI
	{

		void Cpuid::execute(Interpreter& executor, const std::vector<std::string>& arguments, const CommandRegistry& commandRegistry) const
		{
			size_t argsSize = arguments.size();
			const CpuInfo& cpuInfo = CpuInfo::getCpuInfo();
			if (argsSize >= 2)
			{
				for (size_t i = 1; i < argsSize; ++i)
				{
					if (arguments[i] == "info")
					{
						cpuInfo.exportBasicInfo(std::cout);
					}
					else if (arguments[i] == "instructions")
					{
						cpuInfo.exportInstructionSetSupportInfo(std::cout);
					}
					else if (arguments[i] == "caches")
					{
						cpuInfo.exportCacheInfo(std::cout);
					}
					else
					{
						throw BadCommandException("Cpuid", "'" + arguments[i] + "' is not a valid parameter");
					}
				}
			}
			else
			{
				cpuInfo.exportTo(std::cout);
			}

		}

		Cpuid::Cpuid()
				: InterpreterCommand("cpuid")
		{
			_description = "Displays the CPUID info.";
			_help = "Usage: cpuid [info] [instructions] [caches]\n"
					"The cpuid command accepts any of the above argument combination\n"
					"An cpuid command with no arguments is equivalent as if all arguments were passed.\n";
		}

		Cpuid::~Cpuid()
		{
		}

	} /* namespace CLI */
} /* namespace Elpida */

