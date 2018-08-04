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

#include "Commands/Cpuid.hpp"

#include <iostream>

#include "Elpida/CpuInfo.hpp"
#include "BadCommandException.hpp"
#include "Elpida/Utilities/CommandParser.hpp"

namespace Elpida
{
	namespace CLI
	{
		Cpuid::Cpuid()
				: InterpreterCommand("cpuid")
		{
			_description = "Displays the CPUID info.";
			_usage = "cpuid [info] [instructions] [caches]";
			_help = "The cpuid command accepts any of the above argument combination\n"
					"A cpuid command with no arguments is equivalent as if all arguments were passed.\n";
		}

		Cpuid::~Cpuid()
		{
		}

		void Cpuid::execute(Interpreter& executor, const CommandParser& commandParser, const CommandRegistry& commandRegistry) const
		{
			const CpuInfo& cpuInfo = CpuInfo::getCpuInfo();

			if (commandParser.getNamedArguments().size() >= 2)
			{
				if (commandParser.argumentExists("info"))
				{
					cpuInfo.exportBasicInfo(std::cout);
				}

				if (commandParser.argumentExists("instructions"))
				{
					cpuInfo.exportInstructionSetSupportInfo(std::cout);
				}

				if (commandParser.argumentExists("caches"))
				{
					cpuInfo.exportCacheInfo(std::cout);
				}
			}
			else
			{
				cpuInfo.exportTo(std::cout);
			}
		}

	} /* namespace CLI */
} /* namespace Elpida */

