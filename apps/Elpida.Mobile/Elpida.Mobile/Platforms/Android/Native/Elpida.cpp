/*
*  Copyright (c) 2024-2026  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ElpidaInstance.hpp"
#include "FullBenchmarkInstancesLoader.hpp"
#include "JsonSerializer.hpp"
#include "ModelBuilderJson.hpp"
#include "Core/BenchmarkExecutionService.hpp"
#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/MemoryInfoLoader.hpp"
#include "Elpida/Platform/TopologyLoader.hpp"
#include "Elpida/Core/TimingCalculator.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "ScoreCalculator.hpp"
#include "InfoGetter.hpp"

#include <iostream>
#include <thread>
#include <dlfcn.h>
#include <Elpida/Core/Config.hpp>

#include "DynamicLoadedBenchmark.hpp"
#include "Elpida/Platform/AsyncPipeReader.hpp"

using namespace Elpida;
using namespace Elpida::Application;
using namespace nlohmann;

char lastError[512];

extern "C" {
static TopologyNodeType TranslateNativeTopologyType(NodeType nativeNodeType)
{
	switch (nativeNodeType)
	{
	case NodeType::Machine:
		return Elpida::Application::TopologyNodeType::Machine;
	case NodeType::Package:
		return Elpida::Application::TopologyNodeType::Package;
	case NodeType::NumaDomain:
		return Elpida::Application::TopologyNodeType::NumaDomain;
	case NodeType::Group:
		return Elpida::Application::TopologyNodeType::Group;
	case NodeType::Die:
		return Elpida::Application::TopologyNodeType::Die;
	case NodeType::Core:
		return Elpida::Application::TopologyNodeType::Core;
	case NodeType::L1ICache:
		return Elpida::Application::TopologyNodeType::L1ICache;
	case NodeType::L1DCache:
		return Elpida::Application::TopologyNodeType::L1DCache;
	case NodeType::L2ICache:
		return Elpida::Application::TopologyNodeType::L2ICache;
	case NodeType::L2DCache:
		return Elpida::Application::TopologyNodeType::L2DCache;
	case NodeType::L3ICache:
		return Elpida::Application::TopologyNodeType::L3ICache;
	case NodeType::L3DCache:
		return Elpida::Application::TopologyNodeType::L3DCache;
	case NodeType::L4Cache:
		return Elpida::Application::TopologyNodeType::L4Cache;
	case NodeType::L5Cache:
		return Elpida::Application::TopologyNodeType::L5Cache;
	case NodeType::ProcessingUnit:
		return Elpida::Application::TopologyNodeType::ProcessingUnit;
	case NodeType::Unknown:
		break;
	}
	return Elpida::Application::TopologyNodeType::ProcessingUnit;
}

static std::optional<std::size_t> GetEfficiency(const TopologyNode& node)
{
	if (node.GetType() == NodeType::ProcessingUnit)
	{
		auto& cpu = static_cast<const ProcessingUnitNode&>(node);
		if (cpu.GetCpuKind())
		{
			return cpu.GetCpuKind()->get().GetEfficiency();
		}
	}
	return std::nullopt;
}

static std::optional<std::size_t> GetNodeSize(const TopologyNode& node)
{
	switch (node.GetType())
	{
	case NodeType::L1ICache:
	case NodeType::L1DCache:
	case NodeType::L2ICache:
	case NodeType::L2DCache:
	case NodeType::L3ICache:
	case NodeType::L3DCache:
	case NodeType::L4Cache:
	case NodeType::L5Cache:
		return static_cast<const CpuCacheNode&>(node).GetSize();
	case NodeType::NumaDomain:
		return static_cast<const NumaNode&>(node).GetLocalMemorySize();
	default:
		return std::nullopt;
	}
}

static TopologyNodeModel GetTopologyNodeModel(const TopologyNode& node)
{
	std::vector<TopologyNodeModel> children;
	for (auto& child : node.GetChildren())
	{
		children.push_back(GetTopologyNodeModel(*child));
	}

	std::vector<TopologyNodeModel> memoryChildren;
	for (auto& child : node.GetMemoryChildren())
	{
		memoryChildren.push_back(GetTopologyNodeModel(*child));
	}

	return TopologyNodeModel(TranslateNativeTopologyType(node.GetType()),
							 node.GetOsIndex(), GetNodeSize(node),
							 GetEfficiency(node),
							 std::move(children),
							 std::move(memoryChildren));
}

double CalculateTotalScore(double singleCoreScore, double multiCoreScore)
{
	return ScoreCalculator::CalculateTotalScore(singleCoreScore, multiCoreScore);
}

double CalculateScore(const double score[], const double baseScores[], const uint32_t size)
{
	return ScoreCalculator::CalculateBenchmarkScore(score, baseScores, size);
}


static bool EndsWith(std::string const& value, std::string const& ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

static std::vector<BenchmarkModel> GetBenchmarkModels(const std::string& path,
													  const Vector<UniquePtr<Benchmark>>& benchmarks)
{
	std::vector<BenchmarkModel> models;
	auto index = 0;
	for (const auto& benchmark : benchmarks)
	{
		std::vector<BenchmarkConfigurationModel> configurationModels;
		auto info = benchmark->GetInfo();
		for (auto& configuration : benchmark->GetRequiredConfiguration())
		{
			configurationModels.emplace_back(configuration.GetName(),
											 info.GetName() + "/" + configuration.GetName(),
											 configuration.GetValue(),
											 static_cast<Application::ConfigurationType>(configuration.GetType())
			);
		}
		models.emplace_back(info.GetName(),
							info.GetDescription(),
							info.GetResultUnit(),
							info.GetResultType(),
							path,
							index++,
							std::move(configurationModels));
	}
	return models;
}

static std::vector<BenchmarkGroupModel> GetBenchmarkGroups(const std::filesystem::path& path, const std::string& suffix)
{
	if (!is_directory(path))
	{
		return {};
	}

	std::vector<BenchmarkGroupModel> loaded;
	for (auto& entry : std::filesystem::directory_iterator(path))
	{
		if (!entry.is_directory()
			&& entry.is_regular_file()
			&& (suffix.empty() || EndsWith(entry.path().string(), suffix)))
		{
			try
			{
				const auto& exePath = entry.path();
				auto lib = dlopen(exePath.string().c_str(), RTLD_NOW);
				if (lib == nullptr)
				{
					throw ElpidaException("Failed to load benchmark group: ", dlerror());
				}

				auto func = reinterpret_cast<ELPIDA_CREATE_BENCHMARK_GROUP_PTR>(dlsym(lib, "CreateBenchmarkGroup"));
				if (func == nullptr)
				{
					throw ElpidaException("Failed to load benchmark group function: ", dlerror());
				}

				auto loadedGroup = func();
				loaded.emplace_back(loadedGroup->GetName(), GetBenchmarkModels(exePath, loadedGroup->GetBenchmarks()));
			}
			catch (const std::exception& ex)
			{
				// LOlg
			}
		}
	}
	return loaded;
}

ElpidaInstance* Load(char* executableDirectory)
{
	ElpidaInstance* instance = nullptr;
	try
	{
		if (executableDirectory == nullptr)
		{
			std::strncpy(lastError, "input data is null", sizeof(lastError));
			return nullptr;
		}

		auto directory = std::filesystem::path(executableDirectory);

		instance = new ElpidaInstance{
			EnvironmentInfo{
				CpuInfoLoader::Load(),
				MemoryInfoLoader::Load(),
				OsInfoLoader::Load(),
				TopologyLoader::LoadTopology(),
				TimingCalculator::CalculateTiming()
			},
		};

		instance->timingModel = TimingModel(
			instance->environmentInfo.GetOverheadsInfo().GetNowOverhead(),
			instance->environmentInfo.GetOverheadsInfo().GetLoopOverhead(),
			instance->environmentInfo.GetOverheadsInfo().GetIterationsPerSecond()
		);

		instance->memoryModel = MemoryInfoModel(
			instance->environmentInfo.GetMemoryInfo().GetTotalSize(),
			instance->environmentInfo.GetMemoryInfo().GetPageSize()
		);

		instance->osInfoModel = OsInfoModel(
			instance->environmentInfo.GetOsInfo().GetCategory(),
			instance->environmentInfo.GetOsInfo().GetName(),
			instance->environmentInfo.GetOsInfo().GetVersion()
		);

		instance->cpuInfoModel = CpuInfoModel(
			instance->environmentInfo.GetCpuInfo().GetArchitecture(),
			instance->environmentInfo.GetCpuInfo().GetVendorName(),
			instance->environmentInfo.GetCpuInfo().GetModelName()
		);

		instance->topologyModel = TopologyModel(
			GetTopologyNodeModel(instance->environmentInfo.GetTopologyInfo().GetRoot()),
			0
		);

		instance->benchmarkGroupModels = GetBenchmarkGroups(directory, "-group.so");

		instance->benchmarkExecutionService = InProcessBenchmarkExecutionService();
		instance->benchmarkExecutionService.SetElpidaInstance(instance);
		instance->benchmarkRunConfigurationModel = BenchmarkRunConfigurationModel();
		instance->benchmarkExecutionService.SetElpidaInstance(instance);

		std::vector<std::string> missingBenchmarks;
		auto benchmarksLoaded = FullBenchmarkInstancesLoader::Load(
			instance->benchmarkGroupModels,
			instance->timingModel,
			instance->topologyModel,
			instance->memoryModel,
			instance->benchmarkRunConfigurationModel,
			instance->benchmarkExecutionService,
			missingBenchmarks);

		if (!missingBenchmarks.empty())
		{
			std::ostringstream stream;
			for (auto& benchmark : missingBenchmarks)
			{
				stream << benchmark << ", ";
			}
			throw std::runtime_error("Missing benchmarks: " + stream.str());
		}

		instance->benchmarkInstances = std::move(benchmarksLoaded);
		return instance;
	}
	catch (const std::exception& ex)
	{
		auto message = ex.what();

		std::strncpy(lastError, message, sizeof(lastError));
		delete instance;
		return nullptr;
	}
}

const char* GetLastError()
{
	return lastError;
}

void Destroy(const ElpidaInstance* instance)
{
	delete instance;
}

int RunBenchmark(ElpidaInstance* instance,
				 uint64_t fullIndex,
				 double* result)
{
	try
	{
		if (fullIndex >= instance->benchmarkInstances.size())
		{
			std::strncpy(lastError, "Invalid index", sizeof(lastError));
			return EXIT_FAILURE;
		}

		// thread to avoid static init/deinit errors due to dlclose() (mainly openssl)
		std::thread th([&]()
		{
			const auto& benchmarkInstance = instance->benchmarkInstances[fullIndex];
			const std::string actualFilename = benchmarkInstance->GetBenchmark().GetFilePath();
			DynamicLoadedBenchmark library(actualFilename.c_str());
			auto benchmark = library.GetBenchmark(benchmarkInstance->GetBenchmark().GetBenchmarkIndex());
			instance->benchmarkExecutionService.SetBenchmark(benchmark);
			const auto benchmarkResult = benchmarkInstance->Run();
			*result = benchmarkResult.GetScore();
		});

		th.join();
		return EXIT_SUCCESS;
	}
	catch (const std::exception& ex)
	{
		auto message = ex.what();

		std::strncpy(lastError, message, sizeof(lastError));
		return EXIT_FAILURE;
	}
}

static nlohmann::json Serialize(const TopologyNodeModel& topologyNode)
{
	json jNode;

	jNode["type"] = static_cast<int>(topologyNode.GetType());
	if (topologyNode.GetOsIndex().has_value())
	{
		jNode["osIndex"] = topologyNode.GetOsIndex().value();
	}

	switch (topologyNode.GetType())
	{
	case TopologyNodeType::L1ICache:
	case TopologyNodeType::L1DCache:
	case TopologyNodeType::L2ICache:
	case TopologyNodeType::L2DCache:
	case TopologyNodeType::L3ICache:
	case TopologyNodeType::L3DCache:
	case TopologyNodeType::L4Cache:
	case TopologyNodeType::L5Cache:
	case TopologyNodeType::NumaDomain:
		{
			if (topologyNode.GetSize().has_value())
			{
				jNode["size"] = topologyNode.GetSize().value();
			}
		}
		break;
	case TopologyNodeType::ProcessingUnit:
		{
			if (topologyNode.GetEfficiency().has_value())
			{
				jNode["efficiency"] = topologyNode.GetEfficiency().value();
			}
		}
		break;
	default:
		break;
	}

	if (!topologyNode.GetMemoryChildren().empty())
	{
		json memoryChildren = json::array();

		for (auto& child : topologyNode.GetMemoryChildren())
		{
			memoryChildren.push_back(Serialize(child));
		}

		jNode["memoryChildren"] = std::move(memoryChildren);
	}

	if (!topologyNode.GetChildren().empty())
	{
		json children = json::array();

		for (auto& child : topologyNode.GetChildren())
		{
			children.push_back(Serialize(child));
		}

		jNode["children"] = std::move(children);
	}

	return jNode;
}

int GetInfo(ElpidaInstance* instance, char** buffer, uint64_t* size)
{
	try
	{
		json root;
		{
			auto& cpuInfo = instance->cpuInfoModel;
			json cpu;

			cpu["architecture"] = cpuInfo.GetArchitecture();
			cpu["vendor"] = cpuInfo.GetVendorName();
			cpu["modelName"] = cpuInfo.GetModelName();

			root["cpu"] = std::move(cpu);
		}
		{
			auto& memoryInfo = instance->memoryModel;
			json memory;

			memory["pageSize"] = memoryInfo.GetPageSize();
			memory["totalSize"] = memoryInfo.GetTotalSize();

			root["memory"] = std::move(memory);
		}
		{
			auto& osInfo = instance->osInfoModel;
			json os;

			os["category"] = osInfo.GetCategory();
			os["name"] = osInfo.GetName();
			os["version"] = osInfo.GetVersion();

			root["os"] = std::move(os);
		}
		{
			json topology;

			topology["root"] = Serialize(instance->topologyModel.GetRoot());
			topology["fastestProcessor"] = 0;

			root["topology"] = std::move(topology);
		}
		{
			auto& timingInfo = instance->timingModel;

			json jTiming;

			jTiming["iterations"] = timingInfo.GetIterationsPerSecond();
			jTiming["loopOverhead"] = timingInfo.GetLoopOverhead().count();
			jTiming["nowOverhead"] = timingInfo.GetNowOverhead().count();

			root["timing"] = std::move(jTiming);
		}

		{
			json elpidaVersion;
			elpidaVersion["version"] = ELPIDA_VERSION;
			elpidaVersion["compilerName"] = ELPIDA_COMPILER_NAME;
			elpidaVersion["compilerVersion"] = ELPIDA_COMPILER_VERSION;
			root["elpidaVersion"] = std::move(elpidaVersion);
		}

		json benchmarkGroups = json::array();
		for (auto& benchmarkInstance : instance->benchmarkInstances)
		{
			benchmarkInstance->Configure();
			json benchmarkJ;
			benchmarkJ["uuid"] = benchmarkInstance->GetUuid();
			benchmarkJ["name"] = benchmarkInstance->GetName();
			benchmarkJ["baseScore"] = benchmarkInstance->GetBaseScore();
			benchmarkJ["concurrencyMode"] = benchmarkInstance->GetMultiThreadConcurrencyMode();
			benchmarkJ["isMultiThread"] = benchmarkInstance->IsMultiThread();

			auto& benchmark = benchmarkInstance->GetBenchmark();

			json benchmarkInfoJ;
			benchmarkInfoJ["name"] = benchmark.GetName();
			benchmarkInfoJ["description"] = benchmark.GetDescription();
			benchmarkInfoJ["resultType"] = benchmark.GetResultType();
			benchmarkInfoJ["resultUnit"] = benchmark.GetResultUnit();
			benchmarkInfoJ["filename"] = benchmark.GetFilePath();
			benchmarkInfoJ["index"] = benchmark.GetBenchmarkIndex();

			json benchmarkConfigJ = json::array();

			for (auto& configuration : benchmark.GetConfigurations())
			{
				json thisBenchmarkConfigJ;
				thisBenchmarkConfigJ["name"] = configuration.GetName();
				thisBenchmarkConfigJ["id"] = configuration.GetId();
				thisBenchmarkConfigJ["type"] = configuration.GetType();
				thisBenchmarkConfigJ["value"] = configuration.GetValue();
				benchmarkConfigJ.push_back(thisBenchmarkConfigJ);
			}

			benchmarkInfoJ["configurations"] = benchmarkConfigJ;
			benchmarkJ["benchmarkInfo"] = benchmarkInfoJ;

			benchmarkGroups.push_back(benchmarkJ);
		}

		root["benchmarks"] = std::move(benchmarkGroups);

		auto serialized = root.dump();

		*size = serialized.size();
		*buffer = new char[*size];
		std::strncpy(*buffer, serialized.c_str(), *size);

		return EXIT_SUCCESS;
	}
	catch (const std::exception& ex)
	{
		auto message = ex.what();

		std::strncpy(lastError, message, sizeof(lastError));
		return EXIT_FAILURE;
	}
}

void DestroyBuffer(const char* buffer)
{
	delete[] buffer;
}
}
