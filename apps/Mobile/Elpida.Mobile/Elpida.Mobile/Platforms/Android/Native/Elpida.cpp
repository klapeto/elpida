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

#include <iostream>
#include <thread>
#include <Elpida/Core/Config.hpp>

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

		std::string directory(executableDirectory);
		Process process(std::filesystem::path(directory) / "elpida-info-dumper", {directory, "-benchmarks"}, true,
		                true);
		AsyncPipeReader stdOutReader(process.GetStdOut());
		AsyncPipeReader stdErrReader(process.GetStdErr());

		stdOutReader.StartReading();
		stdErrReader.StartReading();
		process.GetStdOut().CloseWrite();
		process.GetStdErr().CloseWrite();
		process.WaitToExit();
		stdOutReader.StopReading();
		stdErrReader.StopReading();

		auto error = stdErrReader.GetString();
		if (!error.empty())
		{
			std::strncpy(lastError, error.c_str(), sizeof(lastError));
			return nullptr;
		}

		instance = new ElpidaInstance{
			ModelBuilderJson(stdOutReader.GetString()),
		};

		std::vector<std::string> missingBenchmarks;

		auto benchmarksLoaded = FullBenchmarkInstancesLoader::Load(
			instance->modelBuilderJson.GetBenchmarkGroups(),
			instance->modelBuilderJson.GetTimingModel(),
			instance->modelBuilderJson.GetTopologyInfoModel(),
			instance->modelBuilderJson.GetMemoryInfoModel(),
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

		auto fullBenchmarkResult = instance->benchmarkInstances[fullIndex]->Run();

		*result = fullBenchmarkResult.GetScore();
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
			auto& cpuInfo = instance->modelBuilderJson.GetCpuInfoModel();
			json cpu;

			cpu["architecture"] = cpuInfo.GetArchitecture();
			cpu["vendor"] = cpuInfo.GetVendorName();
			cpu["modelName"] = cpuInfo.GetModelName();

			root["cpu"] = std::move(cpu);
		}
		{
			auto& memoryInfo = instance->modelBuilderJson.GetMemoryInfoModel();
			json memory;

			memory["pageSize"] = memoryInfo.GetPageSize();
			memory["totalSize"] = memoryInfo.GetTotalSize();

			root["memory"] = std::move(memory);
		}
		{
			auto& osInfo = instance->modelBuilderJson.GetOsInfoModel();
			json os;

			os["category"] = osInfo.GetCategory();
			os["name"] = osInfo.GetName();
			os["version"] = osInfo.GetVersion();

			root["os"] = std::move(os);
		}
		{
			json topology;
			topology["root"] = Serialize(instance->modelBuilderJson.GetTopologyInfoModel().GetRoot());
			topology["fastestProcessor"] = 0;

			root["topology"] = std::move(topology);
		}
		{
			auto& timingInfo = instance->modelBuilderJson.GetTimingModel();
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
