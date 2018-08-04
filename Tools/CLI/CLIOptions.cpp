/**************************************************************************
*   Elpida - Benchmark library
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
 * CLIOptions.cpp
 *
 *  Created on: 26 Ιουν 2018
 *      Author: klapeto
 */

#include "CLIOptions.hpp"

#include <getopt.h>

namespace Elpida
{
	namespace CLI
	{

		void CLIOptions::parseArguments(int argC, char** argV)
		{
			if (argC >= 0 && argV != nullptr)
			{
				option options[] =
					{
						{ "verbose", no_argument, (int*) &_verbose, 'v' },
						{ "help", no_argument, (int*) &_showHelp, 'h' },
						{ "cpuid", no_argument, (int*) &_showCpuId, 0 },
						{ 0, 0, nullptr, 0 } };

				int index = 0;
				int c = getopt_long(argC, argV, "v", options, &index);
				while (c != -1)
				{
					switch (c)
					{
						case 0:
							if (options[index].flag != 0) break;
							if (optarg) break;
							break;
					}
				}
			}
		}

	}  // namespace CLI
} /* namespace Elpida */
