#include "ElpidaInstance.hpp"
#include "FullBenchmarkInstancesLoader.hpp"
#include "InProcessBenchmarkExecutionService.hpp"
#include "JsonSerializer.hpp"
#include "ModelBuilderJson.hpp"
#include "Core/BenchmarkExecutionService.hpp"
#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/MemoryInfoLoader.hpp"
#include "Elpida/Platform/TopologyLoader.hpp"
#include "Elpida/Core/TimingCalculator.hpp"
#include "Elpida/Core/ConcurrencyMode.hpp"
#include "Benchmarks/Compression/ZlibCompressionBenchmark.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "Elpida/Core/DefaultAllocatorFactory.hpp"

#include <iostream>

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
		node.GetOsIndex(),GetNodeSize(node),
		GetEfficiency(node),
		std::move(children),
		std::move(memoryChildren));
}

ElpidaInstance* Load(char* inputJsonData, uint64_t inputSize)
{
	try
	{
		if (inputJsonData == nullptr)
		{
			std::strncpy(lastError, "input data is null", sizeof(lastError));
			return nullptr;
		}
		std::string json(inputJsonData, inputSize);

		auto instance = new ElpidaInstance {
			EnvironmentInfo{
				CpuInfoLoader::Load(),
				MemoryInfoLoader::Load(),
				OsInfoLoader::Load(),
				TopologyLoader::LoadTopology(),
				TimingCalculator::CalculateTiming()
			},
			ModelBuilderJson(json),
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

		instance->topologyModel = TopologyModel(
			GetTopologyNodeModel(instance->environmentInfo.GetTopologyInfo().GetRoot()),
			0
		);
		instance->benchmarkExecutionService = InProcessBenchmarkExecutionService();
		instance->benchmarkExecutionService.SetElpidaInstance(instance);
		instance->benchmarkRunConfigurationModel = BenchmarkRunConfigurationModel();

		instance->benchmarkExecutionService.SetElpidaInstance(instance);

		std::vector<std::string> missingBenchmarks;

		auto benchmarksLoaded = FullBenchmarkInstancesLoader::Load(
			instance->modelBuilderJson.GetBenchmarkGroups(),
			instance->timingModel,
			instance->topologyModel,
			instance->memoryModel,
			instance->benchmarkRunConfigurationModel,
			instance->benchmarkExecutionService,
			missingBenchmarks);

		instance->benchmarkInstances = std::move(benchmarksLoaded);
		return instance;
	}
	catch (const std::exception& ex)
	{
		auto message = ex.what();

		std::strncpy(lastError, message, sizeof(lastError));
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

int RunBenchmark(const ElpidaInstance* instance,
				int index,
                 double* result)
{
	try
	{
		auto res = instance->benchmarkInstances[index]->Run();
		*result = res.GetScore();
		return EXIT_SUCCESS;
	}
	catch (const std::exception& ex)
	{
		auto message = ex.what();

		std::strncpy(lastError, message, sizeof(lastError));
		return EXIT_FAILURE;
	}

}

int GetBenchmarkInstancesSerializedInfo(
	const ElpidaInstance* instance,
	char** outputJsonData, uint64_t* outputSize)
{
	try
	{


		nlohmann::json root = json::array();
		for (auto& benchmarkInstance : instance->benchmarkInstances)
		{
			nlohmann::json benchmarkJ;
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
			benchmarkInfoJ["index"] = benchmark.GetBenchmarkIndex();

			json benchmarkConfigJ = json::array();

			for (auto& configuration : benchmark.GetConfigurations())
			{
				json thisBenchmarkConfigJ;
				thisBenchmarkConfigJ["name"] = configuration.GetName();
				thisBenchmarkConfigJ["id"] = configuration.GetId();
				thisBenchmarkConfigJ["type"] = configuration.GetType();
				thisBenchmarkConfigJ["defaultValue"] = configuration.GetValue();
				benchmarkConfigJ.push_back(thisBenchmarkConfigJ);
			}

			benchmarkInfoJ["configurations"] = benchmarkConfigJ;
			benchmarkJ["benchmarkInfo"] = benchmarkInfoJ;

			root.push_back(benchmarkJ);
		}

		auto str = root.dump();
		*outputSize = str.size();
		*outputJsonData = new char[*outputSize];
		std::strncpy(*outputJsonData, str.c_str(), *outputSize);
		return EXIT_SUCCESS;
	}
	catch (const std::exception& ex)
	{
		auto message = ex.what();

		std::strncpy(lastError, message, sizeof(lastError));
		return EXIT_FAILURE;
	}
}

int GetSystemSerializedInfo(const ElpidaInstance* instance, char** buffer, uint64_t* size)
{
	try
	{
		json root;
		root["cpu"] = JsonSerializer::Serialize(instance->environmentInfo.GetCpuInfo());
		root["memory"] = JsonSerializer::Serialize(instance->environmentInfo.GetMemoryInfo());
		root["os"] = JsonSerializer::Serialize(instance->environmentInfo.GetOsInfo());
		root["topology"] = JsonSerializer::Serialize(instance->environmentInfo.GetTopologyInfo());
		root["topology"]["fastestProcessor"] = 0;
		root["timing"] = JsonSerializer::Serialize(instance->environmentInfo.GetOverheadsInfo());

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
	delete buffer;
}
}
