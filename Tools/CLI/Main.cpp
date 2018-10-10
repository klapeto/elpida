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
 * Main.cpp
 *
 *  Created on: 30 Ιουν 2018
 *      Author: klapeto
 */

#include <xmmintrin.h>
#include <iostream>

#include "Interpreter.hpp"
#include "Elpida/Config.hpp"
#include "TaskBatches/Config.hpp"
#include "CommandRegistry.hpp"
#include "Elpida/PluginLoader.hpp"
#include "Elpida/Utilities/TextRow.hpp"
#include "Elpida/Utilities/TextTable.hpp"

using namespace Elpida;

int main(int argc, char** argv)
{

	_mm_setcsr(_mm_getcsr() | 0x8040);
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

	TextTable infoTable = { TextColumn { "Elpida", 15 }, TextColumn { "", 15 } };
	infoTable.addRow(TextRow { "Version: ", _elpida_version_string });
	infoTable.addRow(TextRow { "Build with: ", _elpida_compiler_string });
	infoTable.setPadding(4);
	infoTable.setDrawBorders(true);
	infoTable.exportTo(std::cout);

	std::cout << "Welcome to Elpida CLI Interpreter! Type 'help' to get general help about commands." << std::endl;

	try
	{
		CLI::CommandRegistry registry(_elpida_plugin_install_path);
		CLI::Interpreter interpreter(registry);

		interpreter.run();
	}
	catch (const ElpidaException& e)
	{
		std::cout << "Unhandled error occured from '" << e.what() << "'. Reason: " << e.getMessage() << std::endl;
	}

	return 0;
}

