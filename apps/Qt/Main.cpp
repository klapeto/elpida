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

#include "Elpida/Core/Config.hpp"

#include <QApplication>
#include <QIcon>
#include <QSplashScreen>
#include <QScreen>

#include "MainWindow.hpp"

#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/MemoryInfoLoader.hpp"
#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Core/OverheadsInfo.hpp"

using namespace Elpida;

#ifdef ELPIDA_UNIX

#include <execinfo.h>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

void segFaultHandler(int sig)
{
	void* array[20];
	size_t size = backtrace(array, 20);

	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

#endif

static void setupPlatformSpecifics()
{
#ifdef ELPIDA_UNIX
	signal(SIGSEGV, segFaultHandler);
	signal(SIGABRT, segFaultHandler);
#endif
}

int main(int argc, char* argv[])
{
	setupPlatformSpecifics();

	QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QCoreApplication::setOrganizationName("Elpida");
	QCoreApplication::setOrganizationDomain(ELPIDA_WEBSITE_URL);
	QCoreApplication::setApplicationName("Elpida Qt");

	QApplication application(argc, argv);

	auto screenSize = QGuiApplication::primaryScreen()->size();
	auto pixmap = QIcon(":/Elpida_Splash_Screen.svg").pixmap(QSize(screenSize.width() / 2, screenSize.height() / 2));
	QSplashScreen splash(pixmap);

	splash.show();
	QApplication::processEvents();

	OsInfo osInfo = OsInfoLoader::Load();
	MemoryInfo memoryInfo = MemoryInfoLoader::Load();
	CpuInfo cpuInfo = CpuInfoLoader::Load();
	OverheadsInfo overheadsInfo(NanoSeconds(561),NanoSeconds(321),NanoSeconds(132));

	MainWindow mainWindow(osInfo, memoryInfo, cpuInfo, overheadsInfo);

	splash.showMessage("Getting CPU info...");

	mainWindow.show();

	splash.finish(&mainWindow);


	return QApplication::exec();
}
