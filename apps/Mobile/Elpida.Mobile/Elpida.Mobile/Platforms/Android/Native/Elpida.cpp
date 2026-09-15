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
#include <Elpida/Core/Config.hpp>

#include "ScoreCalculator.hpp"

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

double CalculateTotalScore(double singleCoreScore, double multiCoreScore)
{
	return ScoreCalculator::CalculateTotalScore(singleCoreScore, multiCoreScore);
}

double CalculateScore(const double score[], const double baseScores[], const uint32_t size)
{
	return ScoreCalculator::CalculateBenchmarkScore(score, baseScores, size);
}

ElpidaInstance* Load(char* inputJsonData, uint64_t inputSize)
{
	ElpidaInstance* instance = nullptr;
	try
	{
		if (inputJsonData == nullptr)
		{
			std::strncpy(lastError, "input data is null", sizeof(lastError));
			return nullptr;
		}
		std::string json(inputJsonData, inputSize);

		instance = new ElpidaInstance {
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

int GetInfo(const ElpidaInstance* instance, char** buffer, uint64_t* size)
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
		
		json elpidaVersion;
		elpidaVersion["version"] = ELPIDA_VERSION;
		elpidaVersion["compilerName"] = ELPIDA_COMPILER_NAME;
		elpidaVersion["compilerVersion"] = ELPIDA_COMPILER_VERSION;
		root["elpidaVersion"] = std::move(elpidaVersion);

		json benchmarkGroups = json::array();
		for (auto& benchmarkInstance : instance->benchmarkInstances)
		{
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

			benchmarkGroups.push_back(benchmarkJ);
		}

		root["benchmarkGroups"] = std::move(benchmarkGroups);

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

// int main()
// {
// 	std::string input = "{\\bn\b  \"\bbenchmarkGroups\b\": {\\bn\b    \"\bfailed\b\": [],\\bn\b    \"\bloaded\b\": [\\bn\b      {\\bn\b        \"\bbenchmarks\b\": [\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bCompiles\b \bC\b++ \bcode\b \bwith\b \bClang\b\",\\bn\b            \"\bindex\b\": \b0\b,\\bn\b            \"\bname\b\": \"\bC\b++ \bCompilation\b\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bFiles\b\"\\bn\b          }\\bn\b        ],\\bn\b        \"\bfilePath\b\": \"\",\\bn\b        \"\bname\b\": \"\bCompilation\b \bbenchmarks\b\"\\bn\b      },\\bn\b      {\\bn\b        \"\bbenchmarks\b\": [\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bParses\b \ban\b \bXML\b \bdocument\b \bto\b \bdetermine\b \bthe\b \bparsing\b \bspeed\b.\",\\bn\b            \"\bindex\b\": \b0\b,\\bn\b            \"\bname\b\": \"\bXML\b \bParsing\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b128\b\",\\bn\b                \"\bname\b\": \"\bFile\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bchars\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bParses\b \ba\b \bJson\b \bdocument\b \bto\b \bdetermine\b \bthe\b \bparsing\b \bspeed\b.\",\\bn\b            \"\bindex\b\": \b1\b,\\bn\b            \"\bname\b\": \"\bJson\b \bParsing\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b128\b\",\\bn\b                \"\bname\b\": \"\bFile\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bchars\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bEncodes\b \bdata\b \bto\b \bbase64\b\",\\bn\b            \"\bindex\b\": \b2\b,\\bn\b            \"\bname\b\": \"\bBase64\b \bEncode\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b4096\b\",\\bn\b                \"\bname\b\": \"\bInput\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bDecodes\b \bbase64\b \bencoded\b \bdata\b\",\\bn\b            \"\bindex\b\": \b3\b,\\bn\b            \"\bname\b\": \"\bBase64\b \bDecode\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b4096\b\",\\bn\b                \"\bname\b\": \"\bInput\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bSearches\b \btext\b \bwith\b \bregex\b\",\\bn\b            \"\bindex\b\": \b4\b,\\bn\b            \"\bname\b\": \"\bRegex\b (\bBoost\b)\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b2048\b\",\\bn\b                \"\bname\b\": \"\bData\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bChars\b\"\\bn\b          }\\bn\b        ],\\bn\b        \"\bfilePath\b\": \"\",\\bn\b        \"\bname\b\": \"\bWeb\b \bbenchmarks\b\"\\bn\b      },\\bn\b      {\\bn\b        \"\bbenchmarks\b\": [\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bCalculates\b \bthe\b \bpeak\b \bmemory\b \bread\b \bbandwidth\b \bby\b \bcontinuously\b \breading\b \ba\b \bmemory\b \bstream\b.\",\\bn\b            \"\bindex\b\": \b0\b,\\bn\b            \"\bname\b\": \"\bMemory\b \bread\b \bbandwidth\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b64\b\",\\bn\b                \"\bname\b\": \"\bMemory\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bCalculates\b \bthe\b \baverage\b \bmemory\b \baccess\b \blatency\b \bby\b \brandomly\b \breading\b \bmemory\b \bregions\b.\",\\bn\b            \"\bindex\b\": \b1\b,\\bn\b            \"\bname\b\": \"\bMemory\b \blatency\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b64\b\",\\bn\b                \"\bname\b\": \"\bMemory\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b1\b,\\bn\b            \"\bresultUnit\b\": \"\bs\b\"\\bn\b          }\\bn\b        ],\\bn\b        \"\bfilePath\b\": \"\",\\bn\b        \"\bname\b\": \"\bMemory\b \bbenchmarks\b\"\\bn\b      },\\bn\b      {\\bn\b        \"\bbenchmarks\b\": [\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bCalculates\b \bthe\b \bDFT\b \bof\b \bvalues\b \bin\b \bplace\b\",\\bn\b            \"\bindex\b\": \b0\b,\\bn\b            \"\bname\b\": \"\bFFT\b \bcalculation\b \bin\b \bplace\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b2048\b\",\\bn\b                \"\bname\b\": \"\bWorkload\b \bsize\b \bin\b \bbytes\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bnumbers\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bCalculates\b \bthe\b \bproduct\b \bbetween\b \b2\b \bmatrices\b \bthat\b \btheir\b \bsizes\b \bare\b \bnot\b \bknown\b \bin\b \badvance\b\",\\bn\b            \"\bindex\b\": \b1\b,\\bn\b            \"\bname\b\": \"\bDynamic\b \bmatrix\b \bmultiplication\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b32\b\",\\bn\b                \"\bname\b\": \"\bMatrix\b \brows\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              },\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b32\b\",\\bn\b                \"\bname\b\": \"\bMatrix\b \bcolumns\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bnumbers\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bCalculates\b \bthe\b \bproduct\b \bbetween\b \b2\b \b32x23\b \bmatrices\b\",\\bn\b            \"\bindex\b\": \b2\b,\\bn\b            \"\bname\b\": \"\bMatrix\b \bmultiplication\b (\b32x32\b)\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bNumbers\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bCalculates\b \bthe\b \binverse\b \bof\b \ba\b \b4x4\b \bmatrix\b\",\\bn\b            \"\bindex\b\": \b3\b,\\bn\b            \"\bname\b\": \"\bMatrix\b \binverse\b (\b4x4\b)\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bNumbers\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bPerforms\b \bNBody\b \bsimulation\b\",\\bn\b            \"\bindex\b\": \b4\b,\\bn\b            \"\bname\b\": \"\bNBody\b \bSimulation\b\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bIterations\b\"\\bn\b          }\\bn\b        ],\\bn\b        \"\bfilePath\b\": \"\",\\bn\b        \"\bname\b\": \"\bMath\b \bbenchmarks\b\"\\bn\b      },\\bn\b      {\\bn\b        \"\bbenchmarks\b\": [\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bEncodes\b \ban\b \bimage\b \bfile\b \bto\b \bPNG\b.\",\\bn\b            \"\bindex\b\": \b0\b,\\bn\b            \"\bname\b\": \"\bPngEncoding\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"./\btestImage\b.\bpng\b\",\\bn\b                \"\bname\b\": \"\bInput\b \bPNG\b \bfile\b\",\\bn\b                \"\btype\b\": \b3\b\\bn\b              },\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"./\btestImage\b.\bout\b.\bpng\b\",\\bn\b                \"\bname\b\": \"\bOutput\b \bPNG\b \bfile\b\",\\bn\b                \"\btype\b\": \b3\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bPixels\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bParses\b \ban\b \bxml\b \bparsed\b \bdocument\b \bto\b \bSVG\b \bDom\b \btree\b.\",\\bn\b            \"\bindex\b\": \b1\b,\\bn\b            \"\bname\b\": \"\bSvg\b \bParsing\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"./\btest\b-\bdata\b.\bsvg\b\",\\bn\b                \"\bname\b\": \"\bInput\b \bSVG\b \bfile\b\",\\bn\b                \"\btype\b\": \b3\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bElements\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bRasterizes\b \ba\b \bcalculated\b \bSvg\b \bdocument\b.\",\\bn\b            \"\bindex\b\": \b2\b,\\bn\b            \"\bname\b\": \"\bSvg\b \bRasterization\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b1\b\",\\bn\b                \"\bname\b\": \"\bShape\b \bcount\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              },\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b1\b.\b0\b\",\\bn\b                \"\bname\b\": \"\bSVG\b \boutput\b \bscale\b\",\\bn\b                \"\btype\b\": \b1\b\\bn\b              },\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b16\b\",\\bn\b                \"\bname\b\": \"\bSVG\b \brasterization\b \bsub\b \bsamples\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bpixels\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bRasterizes\b \ba\b \bcalculated\b \bSvg\b \bdocument\b. \bUses\b \bmultiple\b \bbuffers\b \binstead\b \bof\b \ba\b \bsingle\b.\",\\bn\b            \"\bindex\b\": \b3\b,\\bn\b            \"\bname\b\": \"\bSvg\b \bRasterization\b \b2\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"./\bassets\b/\bElpida_Background\b.\bsvg\b\",\\bn\b                \"\bname\b\": \"\bInput\b \bfile\b\",\\bn\b                \"\btype\b\": \b3\b\\bn\b              },\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b1\b.\b0\b\",\\bn\b                \"\bname\b\": \"\bSVG\b \boutput\b \bscale\b\",\\bn\b                \"\btype\b\": \b1\b\\bn\b              },\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b32\b\",\\bn\b                \"\bname\b\": \"\bSVG\b \brasterization\b \bsub\b \bsamples\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              },\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\btrue\b\",\\bn\b                \"\bname\b\": \"\bMultithreaded\b \binternal\b \boperations\b\",\\bn\b                \"\btype\b\": \b4\b\\bn\b              },\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"./\btest\b-\bdata\b-\bsingle\b.\bpng\b\",\\bn\b                \"\bname\b\": \"\bOutput\b \bPng\b \bfile\b\",\\bn\b                \"\btype\b\": \b3\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bpixels\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bRasterizes\b \ba\b \bcalculated\b \bSvg\b \bdocument\b \bfrom\b \bthe\b \bdisk\b.\",\\bn\b            \"\bindex\b\": \b4\b,\\bn\b            \"\bname\b\": \"\bSvg\b \bRasterization\b \bfrom\b \bfile\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\",\\bn\b                \"\bname\b\": \"\bInput\b \bfile\b\",\\bn\b                \"\btype\b\": \b3\b\\bn\b              },\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b1\b.\b0\b\",\\bn\b                \"\bname\b\": \"\bSVG\b \boutput\b \bscale\b\",\\bn\b                \"\btype\b\": \b1\b\\bn\b              },\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b16\b\",\\bn\b                \"\bname\b\": \"\bSVG\b \brasterization\b \bsub\b \bsamples\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              },\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"./\btest\b-\bdata\b-\bsingle\b.\bpng\b\",\\bn\b                \"\bname\b\": \"\bOutput\b \bPng\b \bfile\b\",\\bn\b                \"\btype\b\": \b3\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bpixels\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bRay\b \btraces\b \ba\b \b3D\b \bscene\b \bto\b \brasterized\b \bimage\b\",\\bn\b            \"\bindex\b\": \b5\b,\\bn\b            \"\bname\b\": \"\b3D\b \bRay\b \bTracing\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b100\b\",\\bn\b                \"\bname\b\": \"\bRender\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bRays\b\"\\bn\b          }\\bn\b        ],\\bn\b        \"\bfilePath\b\": \"\",\\bn\b        \"\bname\b\": \"\bImage\b \bbenchmarks\b\"\\bn\b      },\\bn\b      {\\bn\b        \"\bbenchmarks\b\": [\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bEncrypts\b \bdata\b \bwith\b \ba\b \bpublic\b \bkey\b.\",\\bn\b            \"\bindex\b\": \b0\b,\\bn\b            \"\bname\b\": \"\bRSA\b \bEncryption\b\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bDecrypts\b \bdata\b \bwith\b \ba\b \bprivate\b \bkey\b.\",\\bn\b            \"\bindex\b\": \b1\b,\\bn\b            \"\bname\b\": \"\bRSA\b \bDecryption\b\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bEncrypts\b \bdata\b \bwith\b \ba\b \bAES\b.\",\\bn\b            \"\bindex\b\": \b2\b,\\bn\b            \"\bname\b\": \"\bAES\b \bEncryption\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b131072\b\",\\bn\b                \"\bname\b\": \"\bData\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bDecrypts\b \bdata\b \bwith\b \ba\b \bAES\b\",\\bn\b            \"\bindex\b\": \b3\b,\\bn\b            \"\bname\b\": \"\bAES\b \bDecryption\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b131072\b\",\\bn\b                \"\bname\b\": \"\bData\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          }\\bn\b        ],\\bn\b        \"\bfilePath\b\": \"\",\\bn\b        \"\bname\b\": \"\bEncryption\b \bbenchmarks\b\"\\bn\b      },\\bn\b      {\\bn\b        \"\bbenchmarks\b\": [\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bCompresses\b \ba\b \bdata\b \bwith\b \bzlib\b\",\\bn\b            \"\bindex\b\": \b0\b,\\bn\b            \"\bname\b\": \"\bZlib\b \bcompression\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b32768\b\",\\bn\b                \"\bname\b\": \"\bData\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bDecompresses\b \ba\b \bdata\b \bwith\b \bzlib\b\",\\bn\b            \"\bindex\b\": \b1\b,\\bn\b            \"\bname\b\": \"\bZlib\b \bdecompression\b\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b32768\b\",\\bn\b                \"\bname\b\": \"\bData\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          }\\bn\b        ],\\bn\b        \"\bfilePath\b\": \"\",\\bn\b        \"\bname\b\": \"\bCompression\b \bbenchmarks\b\"\\bn\b      },\\bn\b      {\\bn\b        \"\bbenchmarks\b\": [\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bMeasures\b \bthe\b \bmalloc\b() \bthroughput\b\",\\bn\b            \"\bindex\b\": \b0\b,\\bn\b            \"\bname\b\": \"\bmalloc\b()\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bMeasures\b \bthe\b \bstrtod\b() \bthroughput\b\",\\bn\b            \"\bindex\b\": \b1\b,\\bn\b            \"\bname\b\": \"\bstrtod\b()\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bchars\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bMeasures\b \bthe\b \bstrtoul\b() \bthroughput\b\",\\bn\b            \"\bindex\b\": \b2\b,\\bn\b            \"\bname\b\": \"\bstrtoul\b()\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bchars\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bMeasures\b \bthe\b \bmemset\b() \bthroughput\b\",\\bn\b            \"\bindex\b\": \b3\b,\\bn\b            \"\bname\b\": \"\bmemset\b()\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b268435456\b\",\\bn\b                \"\bname\b\": \"\bMemory\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bMeasures\b \bthe\b \bmemchr\b() \bthroughput\b\",\\bn\b            \"\bindex\b\": \b4\b,\\bn\b            \"\bname\b\": \"\bmemchr\b()\",\\bn\b            \"\brequiredConfiguration\b\": [\\bn\b              {\\bn\b                \"\bdefaultValue\b\": \"\b268435456\b\",\\bn\b                \"\bname\b\": \"\bMemory\b \bsize\b\",\\bn\b                \"\btype\b\": \b0\b\\bn\b              }\\bn\b            ],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bB\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bMeasures\b \bthe\b \bsqrt\b() \bthroughput\b\",\\bn\b            \"\bindex\b\": \b5\b,\\bn\b            \"\bname\b\": \"\bsqrt\b()\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bnumbers\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bMeasures\b \bthe\b \bceil\b() \bthroughput\b\",\\bn\b            \"\bindex\b\": \b6\b,\\bn\b            \"\bname\b\": \"\bceil\b()\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bnumber\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bMeasures\b \bthe\b \bfloor\b() \bthroughput\b\",\\bn\b            \"\bindex\b\": \b7\b,\\bn\b            \"\bname\b\": \"\bfloor\b()\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bnumbers\b\"\\bn\b          },\\bn\b          {\\bn\b            \"\bdescription\b\": \"\bSearches\b \btext\b \bwith\b \bregex\b\",\\bn\b            \"\bindex\b\": \b8\b,\\bn\b            \"\bname\b\": \"\bRegex\b\",\\bn\b            \"\brequiredConfiguration\b\": [],\\bn\b            \"\bresultType\b\": \b0\b,\\bn\b            \"\bresultUnit\b\": \"\bChars\b\"\\bn\b          }\\bn\b        ],\\bn\b        \"\bfilePath\b\": \"\",\\bn\b        \"\bname\b\": \"\bStdLib\b \bBenchmarks\b\"\\bn\b      }\\bn\b    ]\\bn\b  },\\bn\b  \"\bcpu\b\": {\\bn\b    \"\barchitecture\b\": \"\",\\bn\b    \"\bmodelName\b\": \"\",\\bn\b    \"\bvendor\b\": \"\"\\bn\b  },\\bn\b  \"\bmemory\b\": {\\bn\b    \"\bpageSize\b\": \b0\b,\\bn\b    \"\btotalSize\b\": \b0\b\\bn\b  },\\bn\b  \"\bos\b\": {\\bn\b    \"\bcategory\b\": \"\",\\bn\b    \"\bname\b\": \"\",\\bn\b    \"\bversion\b\": \"\"\\bn\b  },\\bn\b  \"\btiming\b\": {\\bn\b    \"\biterations\b\": \b0\b,\\bn\b    \"\bloopOverhead\b\":\b0\b,\\bn\b    \"\bnowOverhead\b\": \b0\b\\bn\b  },\\bn\b  \"\btopology\b\": {\\bn\b    \"\bfastestProcessor\b\": \b0\b,\\bn\b    \"\broot\b\": {\\bn\b      \"\bchildren\b\": [],\\bn\b      \"\bosIndex\b\": \b0\b,\\bn\b      \"\btype\b\": \b0\b\\bn\b    }\\bn\b  }\\bn\b}";
// 	Load(input.data(), input.size());
// 	return 0;
// }
