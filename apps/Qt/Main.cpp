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

#include <filesystem>
#include <iostream>

#include "MainWindow.hpp"
#include "ConfigurationViewPool.hpp"
#include "QtMessageService.hpp"

#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/MemoryInfoLoader.hpp"
#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Platform/TopologyLoader.hpp"
#include "Elpida/Platform/BenchmarkGroupModule.hpp"
#include "Elpida/Core/OverheadsInfo.hpp"

#include "Models/OsInfoModel.hpp"
#include "Models/MemoryInfoModel.hpp"
#include "Models/CpuInfoModel.hpp"
#include "Models/OverheadsModel.hpp"
#include "Models/TopologyModel.hpp"
#include "Models/TopologyNodeModel.hpp"
#include "Models/BenchmarksModel.hpp"
#include "Models/BenchmarkModel.hpp"

#include "Controllers/BenchmarksController.hpp"

#include "Core/BenchmarkExecutionService.hpp"
#include "QtThreadQueue.hpp"
#include "Elpida/Platform/Process.hpp"

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

static Elpida::Application::ConfigurationType TranslateConfigurationType(Elpida::ConfigurationType configurationType)
{
	return (Elpida::Application::ConfigurationType)configurationType;
}

static BenchmarksModel LoadBenchmarks()
{
	auto benchmarksDirectory = std::filesystem::current_path() / "Benchmarks";
	if (!std::filesystem::exists(benchmarksDirectory))
	{
		return BenchmarksModel({});
	}

	std::vector<BenchmarkGroupModel> benchmarkGroups;
	for (auto& entry: std::filesystem::directory_iterator(benchmarksDirectory))
	{
		if (!entry.is_directory() && entry.is_regular_file())
		{
			try
			{
				BenchmarkGroupModule module(entry.path().string());

				auto& benchmarks = module.GetBenchmarkGroup().GetBenchmarks();
				std::vector<BenchmarkModel> benchmarkModels;
				benchmarkModels.reserve(benchmarks.size());

				for (std::size_t i = 0; i < benchmarks.size(); ++i)
				{
					auto& benchmark = benchmarks[i];
					std::vector<BenchmarkConfigurationModel> configurations;
					configurations.reserve(benchmark->GetRequiredConfiguration().size());
					for (auto& config: benchmark->GetRequiredConfiguration())
					{
						configurations.emplace_back(config.GetName(), config.GetValue(), TranslateConfigurationType(config.GetType()));
					}
					benchmarkModels.emplace_back(benchmark->GetInfo().GetName(), entry.path().string(), i, std::move(configurations));
				}

				benchmarkGroups.emplace_back(module.GetBenchmarkGroup().GetName(), std::move(benchmarkModels));
			}
			catch (const std::exception& ex)
			{
				// invalid file
			}
		}
	}

	return BenchmarksModel(std::move(benchmarkGroups));
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

	splash.showMessage("Getting CPU info...");

	CpuInfo cpuInfo = CpuInfoLoader::Load();
	CpuInfoModel cpuInfoModel(cpuInfo.GetArchitecture(), cpuInfo.GetVendorName(), cpuInfo.GetModelName(),
		cpuInfo.GetFeatures(), cpuInfo.GetAdditionalInfo());

	splash.showMessage("Getting overheads info...");

	OverheadsInfo overheadsInfo(NanoSeconds(561), NanoSeconds(321), NanoSeconds(132));
	OverheadsModel overheadsModel(overheadsInfo.GetNowOverhead(), overheadsInfo.GetLoopOverhead(),
		overheadsInfo.GetVirtualCallOverhead());

	splash.showMessage("Getting topology info...");
	TopologyInfo topologyInfo = TopologyLoader::LoadTopology();
	TopologyModel topologyModel(GetNode(topologyInfo.GetRoot()));
	topologyModel.GetRoot().SetParents();

	splash.showMessage("Loading benchmarks...");
	auto benchmarksModel = LoadBenchmarks();
	QtMessageService messageService;
	BenchmarkExecutionService executionService;
	BenchmarksController benchmarksController(benchmarksModel, topologyModel, overheadsModel, executionService, messageService);
	ConfigurationViewPool configurationViewPool;
	MainWindow mainWindow(osInfoModel,
		memoryInfoModel,
		cpuInfoModel,
		overheadsModel,
		topologyModel,
		benchmarksModel,
		benchmarksController,
		configurationViewPool);

	mainWindow.show();

	splash.finish(&mainWindow);

	ThreadQueue::SetCurrent(std::make_shared<QtThreadQueue>());
	ThreadQueue::Current().lock()->Run();
	//return QApplication::exec();
	return EXIT_SUCCESS;
}
