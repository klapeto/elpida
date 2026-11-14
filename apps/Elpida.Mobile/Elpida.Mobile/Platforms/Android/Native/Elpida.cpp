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
#include "ScoreCalculator.hpp"
#include "InfoGetter.hpp"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <sched.h>
#include <unistd.h>
#include <Elpida/Core/Config.hpp>
#include <Elpida/BenchmarksVersion.hpp>

using namespace Elpida;
using namespace Elpida::Application;
using namespace nlohmann;

char lastError[512];

// Number of CPUs in a CPU list such as "0-3,6,7". -1 if it can't be parsed.
static int CountCpuList(const std::string& list)
{
	int count = 0;
	std::istringstream ranges(list);
	std::string range;
	while (std::getline(ranges, range, ','))
	{
		auto dash = range.find('-');
		try
		{
			if (dash == std::string::npos)
			{
				std::stoi(range);
				count += 1;
			}
			else
			{
				count += std::stoi(range.substr(dash + 1)) - std::stoi(range.substr(0, dash)) + 1;
			}
		}
		catch (const std::exception&)
		{
			return -1;
		}
	}
	return count;
}

// Number of CPUs in a sysfs CPU list file (/sys/devices/system/cpu/online). -1 if unreadable.
static int CountCpuListFile(const char* path)
{
	std::ifstream file(path);
	std::string list;
	if (!file || !std::getline(file, list))
	{
		return -1;
	}
	return CountCpuList(list);
}

// The CPUs the calling thread may run on: Cpus_allowed_list of /proc/<pid>/task/<tid>/status. -1 if unreadable.
static int CountAllowedCpus()
{
	std::ifstream file("/proc/self/task/" + std::to_string(gettid()) + "/status");
	std::string line;
	const std::string key = "Cpus_allowed_list:";
	while (file && std::getline(file, line))
	{
		if (line.compare(0, key.size(), key) == 0)
		{
			auto start = line.find_first_not_of(" \t", key.size());
			return start == std::string::npos ? -1 : CountCpuList(line.substr(start));
		}
	}
	return -1;
}

extern "C" {
// Which CPUs this process (and the benchmark processes it starts, which inherit it) may run on. Android decides that per
// app state (cpusets: top-app, foreground, background, ...).
// allowed: the CPUs this thread is permitted to run on (cpuset and affinity). Read from Cpus_allowed_list and not from
// sched_getaffinity(), which also leaves out CPUs the kernel has paused (idle big cores on recent Android kernels; they
// stay "online" in sysfs and come back under load): that made 6 of 8 cores look restricted on an idle phone.
// online/possible: from sysfs, -1 if unknown.
int GetCpuAvailability(int* allowed, int* online, int* possible)
{
	if (allowed == nullptr || online == nullptr || possible == nullptr)
	{
		std::strncpy(lastError, "input data is null", sizeof(lastError));
		return -1;
	}

	*allowed = CountAllowedCpus();
	if (*allowed < 0)
	{
		// Fallback: sched_getaffinity() (may undercount paused CPUs, see above)
		cpu_set_t set;
		CPU_ZERO(&set);
		if (sched_getaffinity(0, sizeof(set), &set) == -1)
		{
			std::snprintf(lastError, sizeof(lastError), "sched_getaffinity failed: %s", std::strerror(errno));
			return -1;
		}
		*allowed = CPU_COUNT(&set);
	}

	*online = CountCpuListFile("/sys/devices/system/cpu/online");
	*possible = CountCpuListFile("/sys/devices/system/cpu/possible");
	return 0;
}

double CalculateTotalScore(double singleCoreScore, double multiCoreScore)
{
	return ScoreCalculator::CalculateTotalScore(singleCoreScore, multiCoreScore);
}

double CalculateScore(const double score[], const double baseScores[], const uint32_t size)
{
	return ScoreCalculator::CalculateBenchmarkScore(score, baseScores, size);
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

		// Same flow as the desktop CLI/Qt apps (docs/development/android.md): the system information, the timing
		// overheads and the benchmark list come from the elpida-info-dumper executable, and every benchmark runs as its
		// own process (OffProcessBenchmarkExecutionService). The executables are packaged as lib*.so so that Android
		// extracts them to the (executable) native library directory.
		auto directory = std::filesystem::path(executableDirectory);

		InfoGetter infoGetter;
		infoGetter.SetInfoGetterPath(directory / "libelpida-info-dumper.so");
		infoGetter.SetBenchmarksPath(directory);
		infoGetter.SetBenchmarksSuffix("-benchmarks.so");
		// Android limits the CPUs an app may run on (cpusets), so the dumper's search for the fastest core (pinning
		// to every core in turn) fails on many phones. Don't pin, as the in-process code before didn't either
		// (fastestProcessor = 0).
		infoGetter.SetNoThreadPinning(true);
		ModelBuilderJson builderJson(infoGetter.GetData());

		// the models are move-only; builderJson is not used after this
		instance = new ElpidaInstance{
			std::move(builderJson.GetTimingModel()),
			std::move(builderJson.GetTopologyInfoModel()),
			std::move(builderJson.GetMemoryInfoModel()),
			std::move(builderJson.GetOsInfoModel()),
			std::move(builderJson.GetCpuInfoModel()),
		};

		instance->benchmarkGroupModels = std::move(builderJson.GetBenchmarkGroups());
		instance->benchmarkRunConfigurationModel = BenchmarkRunConfigurationModel();

		if (!builderJson.GetFailedToLoadBenchmarkGroups().empty())
		{
			std::ostringstream stream;
			for (auto& [file, reason] : builderJson.GetFailedToLoadBenchmarkGroups())
			{
				stream << file << ": " << reason << ", ";
			}
			throw std::runtime_error("Failed to load benchmark groups: " + stream.str());
		}

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

		const auto benchmarkResult = instance->benchmarkInstances[fullIndex]->Run();
		*result = benchmarkResult.GetScore();

		return EXIT_SUCCESS;
	}
	catch (const std::exception& ex)
	{
		auto message = ex.what();

		std::strncpy(lastError, message, sizeof(lastError));
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::strncpy(lastError, "Unknown benchmark error", sizeof(lastError));
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
			elpidaVersion["benchmarksVersion"] = ELPIDA_BENCHMARKS_VERSION;
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
