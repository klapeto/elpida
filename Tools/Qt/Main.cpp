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

#include <QApplication>
#include <Elpida/Config.hpp>
#include "Tools/Qt/MainWindow.hpp"

#if _elpida_linux
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void segFaultHandler(int sig)
{
	void *array[20];
	size_t size = backtrace(array, 20);

	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}
#endif

int main(int argc, char *argv[])
{
#if _elpida_linux
	signal(SIGSEGV, segFaultHandler);
#endif
	QApplication application(argc, argv);
	Elpida::MainWindow mainWindow;
	mainWindow.show();

	return application.exec();
}
