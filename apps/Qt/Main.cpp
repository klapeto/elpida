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
#include "Elpida/Platform/TopologyLoader.hpp"
#include "Elpida/Core/OverheadsInfo.hpp"

#include "Models/OsInfoModel.hpp"
#include "Models/MemoryInfoModel.hpp"
#include "Models/CpuInfoModel.hpp"
#include "Models/OverheadsModel.hpp"
#include "Models/TopologyModel.hpp"
#include "Models/TopologyNodeModel.hpp"

using namespace Elpida;
using namespace Elpida::Application;

#ifdef ELPIDA_UNIX

#include <execinfo.h>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

void segFaultHandler(int sig)
{
	void* array[20];
	int size = backtrace(array, 20);

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

static TopologyNodeType TranslateType(NodeType nodeType)
{
	switch (nodeType)
	{
	case Machine:
		return Elpida::Application::TopologyNodeType::Machine;
	case Package:
		return Elpida::Application::TopologyNodeType::Package;
	case NumaDomain:
		return Elpida::Application::TopologyNodeType::NumaDomain;
	case Group:
		return Elpida::Application::TopologyNodeType::Group;
	case Die:
		return Elpida::Application::TopologyNodeType::Die;
	case Core:
		return Elpida::Application::TopologyNodeType::Core;
	case L1ICache:
		return Elpida::Application::TopologyNodeType::L1ICache;
	case L1DCache:
		return Elpida::Application::TopologyNodeType::L1DCache;
	case L2ICache:
		return Elpida::Application::TopologyNodeType::L2ICache;
	case L2DCache:
		return Elpida::Application::TopologyNodeType::L2DCache;
	case L3ICache:
		return Elpida::Application::TopologyNodeType::L3ICache;
	case L3DCache:
		return Elpida::Application::TopologyNodeType::L3DCache;
	case L4Cache:
		return Elpida::Application::TopologyNodeType::L4Cache;
	case L5Cache:
		return Elpida::Application::TopologyNodeType::L5Cache;
	case ProcessingUnit:
		return Elpida::Application::TopologyNodeType::ProcessingUnit;
	case Unknown:
		break;
	}
	return Elpida::Application::TopologyNodeType::ProcessingUnit;
}

static std::optional<std::size_t> GetSize(const TopologyNode& node)
{
	std::optional<std::size_t> size;
	switch (node.GetType())
	{
	case NumaDomain:
		size = static_cast<const NumaNode&>(node).GetLocalMemorySize();
		break;
	case L1ICache:
	case L1DCache:
	case L2ICache:
	case L2DCache:
	case L3ICache:
	case L3DCache:
	case L4Cache:
	case L5Cache:
		size = static_cast<const CpuCacheNode&>(node).GetSize();
		break;
	case Group:
	case Die:
	case Core:
	case Machine:
	case Package:
	case ProcessingUnit:
	case Unknown:
		break;
	}

	return size;
}

static TopologyNodeModel GetNode(const TopologyNode& node)
{
	auto& originalMemoryChildren = node.GetMemoryChildren();
	std::vector<TopologyNodeModel> memoryChildren;
	memoryChildren.reserve(originalMemoryChildren.size());

	for (auto& child: originalMemoryChildren)
	{
		memoryChildren.push_back(GetNode(*child));
	}


	auto& originalChildren = node.GetChildren();
	std::vector<TopologyNodeModel> children;
	memoryChildren.reserve(children.size());
	for (auto& child: originalChildren)
	{
		children.push_back(GetNode(*child));
	}

	return TopologyNodeModel(TranslateType(node.GetType()), node.GetOsIndex(), GetSize(node), std::move(children),
			std::move(memoryChildren));
}

TopologyNodeModel GetDummyTopologyNode()
{
	TopologyNodeModel eu1{
			TopologyNodeType::ProcessingUnit,
			0,
			std::nullopt,
			{},
			{}
	};

	TopologyNodeModel eu2{
			TopologyNodeType::ProcessingUnit,
			1,
			std::nullopt,
			{},
			{}
	};

	TopologyNodeModel eu3{
			TopologyNodeType::ProcessingUnit,
			2,
			std::nullopt,
			{},
			{}
	};

	TopologyNodeModel eu4{
			TopologyNodeType::ProcessingUnit,
			3,
			std::nullopt,
			{},
			{}
	};

	TopologyNodeModel eu5{
			TopologyNodeType::ProcessingUnit,
			3,
			std::nullopt,
			{},
			{}
	};

	TopologyNodeModel eu6{
			TopologyNodeType::ProcessingUnit,
			3,
			std::nullopt,
			{},
			{}
	};

	TopologyNodeModel numa{
			TopologyNodeType::NumaDomain,
			1,
			std::nullopt,
			{},
			{}
	};

	TopologyNodeModel core{
			TopologyNodeType::Core,
			1,
			std::nullopt,
			{},
			{}
	};

	TopologyNodeModel core2{
			TopologyNodeType::Core,
			1,
			std::nullopt,
			{},
			{}
	};

	TopologyNodeModel package{
			TopologyNodeType::Package,
			std::nullopt,
			std::nullopt,
			{},
			{}
	};

	TopologyNodeModel machine{
			TopologyNodeType::Machine,
			std::nullopt,
			std::nullopt,
			{},
			{}
	};

	core.GetChildren().push_back(std::move(eu1));
	core.GetChildren().push_back(std::move(eu2));
	core.GetChildren().push_back(std::move(eu3));
	core.GetChildren().push_back(std::move(eu4));
	core.GetChildren().push_back(std::move(eu5));
	core.GetChildren().push_back(std::move(eu6));
	//core.GetMemoryChildren().push_back(std::move(numa));
	//package.GetChildren().push_back(std::move(core));
	machine.GetChildren().push_back(std::move(core));

	return machine;
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
	OsInfoModel osInfoModel(osInfo.GetCategory(), osInfo.GetName(), osInfo.GetVersion());

	MemoryInfo memoryInfo = MemoryInfoLoader::Load();
	MemoryInfoModel memoryInfoModel(memoryInfo.GetTotalSize(), memoryInfo.GetPageSize());

	CpuInfo cpuInfo = CpuInfoLoader::Load();
	CpuInfoModel cpuInfoModel(cpuInfo.GetArchitecture(), cpuInfo.GetVendorName(), cpuInfo.GetModelName(),
			cpuInfo.GetFeatures(), cpuInfo.GetAdditionalInfo());

	OverheadsInfo overheadsInfo(NanoSeconds(561), NanoSeconds(321), NanoSeconds(132));
	OverheadsModel overheadsModel(overheadsInfo.GetNowOverhead(), overheadsInfo.GetLoopOverhead(),
			overheadsInfo.GetVirtualCallOverhead());

	TopologyInfo topologyInfo = TopologyLoader::LoadTopology();
	TopologyModel topologyModel(GetNode(topologyInfo.GetRoot()));
	//TopologyModel topologyModel(GetDummyTopologyNode());

	MainWindow mainWindow(osInfoModel, memoryInfoModel, cpuInfoModel, overheadsModel, topologyModel);

	splash.showMessage("Getting CPU info...");

	mainWindow.show();

	splash.finish(&mainWindow);


	return QApplication::exec();
}
