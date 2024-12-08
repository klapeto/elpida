//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 15/4/2024.
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <filesystem>

#include "JsonSerializer.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/MemoryInfoLoader.hpp"
#include "Elpida/Platform/TopologyLoader.hpp"
#include "Elpida/Core/TimingCalculator.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Platform/Process.hpp"
#include "Elpida/Platform/AsyncPipeReader.hpp"

#include "FullBenchmarkInstancesLoader.hpp"
#include "ModelBuilderJson.hpp"
#include "Core/BenchmarkExecutionService.hpp"

using namespace nlohmann;
using namespace Elpida;
using namespace Elpida::Application;

static std::string GetBenchmarkInfo(const std::filesystem::path& path)
{
	Process process(path.string(), { "--dump-info" }, true, true);
	AsyncPipeReader outReader(process.GetStdOut());
	AsyncPipeReader errReader(process.GetStdErr());
	outReader.StartReading();
	errReader.StartReading();
	process.GetStdOut().CloseWrite();
	process.GetStdErr().CloseWrite();

	try
	{
		process.WaitToExit();
		outReader.StopReading();
		return outReader.GetString();
	}
	catch (...)
	{
		outReader.StopReading();
		errReader.StopReading();
		auto err = errReader.GetString();
		if (!err.empty())
		{
			throw ElpidaException(err);
		}
		else
		{
			throw;
		}
	}
}

static bool IsExecutable(const std::filesystem::directory_entry& entry)
{
#if defined(ELPIDA_UNIX)
	auto permissions = entry.status().permissions();
	return (permissions & std::filesystem::perms::owner_exec) != std::filesystem::perms::none
		   || (permissions & std::filesystem::perms::group_exec) != std::filesystem::perms::none
		   || (permissions & std::filesystem::perms::others_exec) != std::filesystem::perms::none;
#else
	auto path = entry.path();
	return path.has_extension() && path.extension().string() == ".exe";
#endif
}

static json SerializeBenchmarkGroups(const std::filesystem::path& path)
{
	if (!is_directory(path))
	{
		return {};
	}

	json rootJ;
	json loaded = json::array();
	json failedToLoad = json::array();

	for (auto& entry : std::filesystem::directory_iterator(path))
	{
		if (!entry.is_directory() && entry.is_regular_file() && IsExecutable(entry))
		{
			try
			{
				const auto& exePath = entry.path();
				auto strOut = GetBenchmarkInfo(exePath);
				auto json = json::parse(strOut);
				json["filePath"] = exePath.string();
				loaded.push_back(std::move(json));    //validation?
			}
			catch (const std::exception& ex)
			{
				json failedGroup;
				failedGroup["file"] = entry.path().string();
				failedGroup["reason"] = ex.what();
				failedToLoad.push_back(std::move(failedGroup));
			}
		}
	}

	rootJ["loaded"] = std::move(loaded);
	rootJ["failed"] = std::move(failedToLoad);

	return rootJ;
}

static json GetJsonWithOnlyBenchmarks(const std::filesystem::path& benchmarkPath)
{

	TimingInfo timing;
	MemoryInfo memoryInfo(0, 0);
	TopologyInfo topology(std::make_unique<TopologyNode>(NodeType::Machine), {}, {}, {}, {}, {}, nullptr);
	OsInfo osInfo("", "", "");
	CpuInfo cpuInfo("", "", "");

	json root;
	root["cpu"] = JsonSerializer::Serialize(cpuInfo);
	root["memory"] = JsonSerializer::Serialize(memoryInfo);
	root["os"] = JsonSerializer::Serialize(osInfo);
	root["topology"] = JsonSerializer::Serialize(topology);
	root["topology"]["fastestProcessor"] = 0;
	root["timing"] = JsonSerializer::Serialize(timing);
	root["benchmarkGroups"] = SerializeBenchmarkGroups(benchmarkPath);

	return root;
}

int main(int argC, char** argV)
{
	std::filesystem::path benchmarkPath;

	if (argC > 1)
	{
		benchmarkPath = argV[1];
	}
	else
	{
		benchmarkPath = OsUtilities::GetExecutableDirectory() / "Benchmarks";
	}

	try
	{
		// The way this works is completely stupid but works and probably needs to work only once in this era.

		ModelBuilderJson builderJson(GetJsonWithOnlyBenchmarks(benchmarkPath).dump());

		BenchmarkExecutionService service;
		BenchmarkRunConfigurationModel runConfigurationModel;

		std::vector<std::string> missingBenchmarks;
		auto instances = FullBenchmarkInstancesLoader::Load(builderJson.GetBenchmarkGroups(),
				builderJson.GetTimingModel(),
				builderJson.GetTopologyInfoModel(),
				builderJson.GetMemoryInfoModel(),
				runConfigurationModel,
				service,
				missingBenchmarks);

		json root = json::array();

		for (auto& instance : instances)
		{
			json thisInstance;

			auto& benchmark = instance->GetBenchmark();

			thisInstance["benchmarkName"] = instance->GetBenchmark().GetName();
			thisInstance["name"] = instance->GetName();
			thisInstance["description"] = benchmark.GetDescription();
			thisInstance["resultType"] = benchmark.GetResultType();
			thisInstance["resultUnit"] = benchmark.GetResultUnit();
			thisInstance["baseScore"] = instance->GetBaseScore();
			thisInstance["isMultiThreaded"] = instance->IsMultiThread();
			thisInstance["uuid"] = instance->GetUuid();

			root.push_back(std::move(thisInstance));
		}

		std::cout << root.dump();
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}