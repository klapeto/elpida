//
// Created by klapeto on 16/4/2024.
//

#include "ModelBuilderJson.hpp"

#include "json.hpp"

namespace Elpida::Application
{
	static const nlohmann::json& GetOptional(const nlohmann::json& json, const std::string& key)
	{
		static auto defaultEmpty = nlohmann::json();
		return json.contains(key) ? json[key] : defaultEmpty;
	}

	static CpuInfoModel ParseCpuInfo(const nlohmann::json& root)
	{
		auto& cpu = root.at("cpu");

		return {
				cpu.at("architecture").get<std::string>(),
				cpu.at("vendor").get<std::string>(),
				cpu.at("modelName").get<std::string>()
		};
	}

	static OsInfoModel ParseOsInfo(const nlohmann::json& root)
	{
		auto& os = root.at("os");

		return {
				os.at("category").get<std::string>(),
				os.at("name").get<std::string>(),
				os.at("version").get<std::string>()
		};
	}

	static MemoryInfoModel ParseMemoryInfo(const nlohmann::json& root)
	{
		auto& memory = root.at("memory");

		return {
				memory.at("totalSize").get<std::size_t>(),
				memory.at("pageSize").get<std::size_t>()
		};
	}

	static TopologyNodeModel ParseTopologyNode(const nlohmann::json& jNode)
	{
		std::vector<TopologyNodeModel> memoryChildren;
		auto jMemoryChildren = GetOptional(jNode, "memoryChildren");

		if (!jMemoryChildren.is_null() && jMemoryChildren.is_array())
		{
			memoryChildren.reserve(jMemoryChildren.size());
			for (auto& jChild: jMemoryChildren)
			{
				memoryChildren.push_back(ParseTopologyNode(jChild));
			}
		}

		std::vector<TopologyNodeModel> children;
		auto jChildren = GetOptional(jNode, "children");

		if (!jChildren.is_null() && jChildren.is_array())
		{
			children.reserve(jChildren.size());
			for (auto& jChild: jChildren)
			{
				children.push_back(ParseTopologyNode(jChild));
			}
		}

		std::optional<std::size_t> osIndex;
		auto jOsIndex = GetOptional(jNode, "osIndex");
		if (!jOsIndex.is_null())
		{
			osIndex = jOsIndex.get<std::size_t>();
		}

		std::optional<std::size_t> size;
		auto jSize = GetOptional(jNode, "size");
		if (!jSize.is_null())
		{
			size = jSize.get<std::size_t>();
		}

		std::optional<int> efficiency;
		auto jEfficiency = GetOptional(jNode, "efficiency");
		if (!jEfficiency.is_null())
		{
			efficiency = jEfficiency.get<int>();
		}

		return {
				TranslateNativeTopologyType(jNode.at("type").get<NativeNodeType>()),
				osIndex,
				size,
				efficiency,
				std::move(children),
				std::move(memoryChildren),
		};
	}

	static TopologyModel ParseTopologyInfo(const nlohmann::json& root)
	{
		auto& topology = root.at("topology");

		return TopologyModel(ParseTopologyNode(topology.at("root")), topology.at("fastestProcessor").get<unsigned int>());
	}

	static TaskModel ParseTask(const nlohmann::json& jTask)
	{
		return {
				jTask.at("name").get<std::string>(),
				jTask.at("resultUnit").get<std::string>(),
				jTask.at("resultType").get<ResultType>(),
				jTask.at("isMeasured").get<bool>(),
		};
	}

	static std::string RemoveSpaces(const std::string& name)
	{
		std::ostringstream stream;

		for (auto c: name)
		{
			if (isspace(c)) continue;
			stream << c;
		}
		return stream.str();
	}

	static BenchmarkModel ParseBenchmark(const nlohmann::json& jBenchmark, const std::string& path)
	{
		auto benchmarkName = jBenchmark.at("name").get<std::string>();

		std::vector<BenchmarkConfigurationModel> configurationModels;
		auto jConfiguration = GetOptional(jBenchmark, "requiredConfiguration");

		auto benchmarkId = RemoveSpaces(benchmarkName);

		if (!jConfiguration.is_null() && jConfiguration.is_array())
		{
			configurationModels.reserve(jConfiguration.size());
			for (auto& jConfig: jConfiguration)
			{
				auto name = jConfig.at("name").get<std::string>();
				auto settingName = benchmarkId;
				configurationModels.emplace_back(
						name,
						settingName.append("/").append(RemoveSpaces(name)),
						jConfig.at("defaultValue").get<std::string>(),
						jConfig.at("type").get<ConfigurationType>()
				);
			}
		}

		return {
				std::move(benchmarkName),
				jBenchmark.at("description").get<std::string>(),
				jBenchmark.at("resultUnit").get<std::string>(),
				jBenchmark.at("resultType").get<ResultType>(),
				path,
				jBenchmark.at("index").get<std::size_t>(),
				std::move(configurationModels)
		};
	}

	static BenchmarkGroupModel ParseBenchmarkGroup(const nlohmann::json& jGroup)
	{
		auto& jBenchmarks = jGroup.at("benchmarks");
		if (!jBenchmarks.is_array())
		{
			throw ElpidaException("Unexpected info for 'benchmark'. expected array");
		}

		std::vector<BenchmarkModel> benchmarks;
		benchmarks.reserve(jBenchmarks.size());

		auto path = jGroup.at("filePath").get<std::string>();

		for (auto& jBenchmark: jBenchmarks)
		{
			benchmarks.push_back(ParseBenchmark(jBenchmark, path));
		}

		return {
				jGroup.at("name").get<std::string>(),
				std::move(benchmarks)
		};
	}

	std::vector<BenchmarkGroupModel> ParseBenchmarkGroups(const nlohmann::json& root, std::vector<std::tuple<std::string, std::string>>& failedGroups)
	{
		auto& jGroups = root.at("benchmarkGroups");
		if (jGroups.is_null()) return {};
		auto& loaded = jGroups["loaded"];

		if (!loaded.is_array()) throw ElpidaException("Unexpected info for 'benchmarkGroups'. expected array");

		std::vector<BenchmarkGroupModel> benchmarkGroups;
		benchmarkGroups.reserve(loaded.size());

		for (auto& jGroup: loaded)
		{
			benchmarkGroups.push_back(ParseBenchmarkGroup(jGroup));
		}

		auto& failedToLoad = jGroups["failed"];

		if (failedToLoad.is_array())
		{
			for (auto& jGroup: failedToLoad)
			{
				failedGroups.emplace_back(jGroup["file"].get<std::string>(), jGroup["reason"].get<std::string>());
			}
		}


		return benchmarkGroups;
	}

	static TimingModel ParseTimingInfo(const nlohmann::json& root)
	{
		auto& jTiming = root.at("timing");
		if (jTiming.is_null())
		{
			throw ElpidaException("Missing info data: 'timing'");
		}
		return {
			Seconds(jTiming.at("nowOverhead").get<double>()),
			Seconds(jTiming.at("loopOverhead").get<double>()),
			jTiming.at("iterations").get<Iterations>(),
		};
	}

	ModelBuilderJson::ModelBuilderJson(const std::string& jsonData)
	{
		nlohmann::json root = nlohmann::json::parse(jsonData);
		_cpuInfoModel = ParseCpuInfo(root);
		_memoryInfoModel = ParseMemoryInfo(root);
		_osInfoModel = ParseOsInfo(root);
		_topologyInfoModel = ParseTopologyInfo(root);
		_benchmarkGroups = ParseBenchmarkGroups(root, _failedToLoadBenchmarkGroups);
		_topologyInfoModel.GetRoot().SetRelations();
		_timingModel = ParseTimingInfo(root);
	}

	OsInfoModel& ModelBuilderJson::GetOsInfoModel()
	{
		return _osInfoModel;
	}

	MemoryInfoModel& ModelBuilderJson::GetMemoryInfoModel()
	{
		return _memoryInfoModel;
	}

	TopologyModel& ModelBuilderJson::GetTopologyInfoModel()
	{
		return _topologyInfoModel;
	}

	TimingModel& ModelBuilderJson::GetTimingModel()
	{
		return _timingModel;
	}

	CpuInfoModel& ModelBuilderJson::GetCpuInfoModel()
	{
		return _cpuInfoModel;
	}

	std::vector<BenchmarkGroupModel>& ModelBuilderJson::GetBenchmarkGroups()
	{
		return _benchmarkGroups;
	}

	const std::vector<std::tuple<std::string, std::string>>& ModelBuilderJson::GetFailedToLoadBenchmarkGroups() const
	{
		return _failedToLoadBenchmarkGroups;
	}
} // Elpida