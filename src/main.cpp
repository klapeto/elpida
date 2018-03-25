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

#include <xmmintrin.h>
#include <iostream>

#include "Config.hpp"
#include "CpuInfo.hpp"
#include "Runner.hpp"
#include "Utilities/TextRow.hpp"
#include "Utilities/TextTable.hpp"
#include "Tasks/Image/ImageTaskBatch.hpp"

#include <thread>
#include <unistd.h>

using namespace Elpida;

int main(int argc, char** argv)
{

	_mm_setcsr(_mm_getcsr() | 0x8040);
	_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

	TextTable<2> infoTable = { TextColumn { "Elpida", 15 }, TextColumn { "", 15 } };
	infoTable.addRow(TextRow<2> { "Version:", _elpida_version_string });
	infoTable.addRow(TextRow<2> { "Build with:", _elpida_compiler_string });
	infoTable.setPadding(4);
	infoTable.setDrawBorders(true);
	infoTable.exportTo(std::cout);

	CpuInfo::getCpuInfo().exportTo(std::cout);

	Runner runner;

	runner.addTaskBatch(ImageTaskBatch("resources/Images/arta.png", "out.png"));
	runner.executeTasks();


	return 0;
}

