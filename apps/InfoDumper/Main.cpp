//
// Created by klapeto on 15/4/2024.
//

#include <cstdlib>
#include <iostream>
#include <string>
#include <filesystem>

#include "JsonSerializer.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/MemoryInfoLoader.hpp"
#include "Elpida/Platform/TopologyLoader.hpp"
#include "Elpida/Core/TimingCalculator.hpp"
#include "Elpida/Platform/BenchmarkGroupModule.hpp"

using namespace nlohmann;
using namespace Elpida;

static json SerializeBenchmarkGroup(const BenchmarkGroup& group, const std::string& modulePath)
{
	auto value = JsonSerializer::Serialize(group);
	value["filePath"] = modulePath;

	std::size_t i = 0;
	for (auto& benchmark : value["benchmarks"])
	{
		benchmark["index"] = i++;
	}
	return value;
}

static json SerializeBenchmarkGroups(const std::filesystem::path& path)
{
	if (!is_directory(path))
	{
		return {};
	}

	json benchmarkGroups = json::array();

	for (auto& entry: std::filesystem::directory_iterator(path))
	{
		if (!entry.is_directory() && entry.is_regular_file())
		{
			try
			{
				auto strPath = entry.path().string();
				BenchmarkGroupModule module(strPath);

				benchmarkGroups.push_back(SerializeBenchmarkGroup(module.GetBenchmarkGroup(), strPath));
			}
			catch (const std::exception& ex)
			{
				// invalid file
			}
		}
	}

	return benchmarkGroups;
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

		auto topology = TopologyLoader::LoadTopology();

		auto timing = TimingCalculator::CalculateTiming(topology);

		json root;
		root["cpu"] = JsonSerializer::Serialize(CpuInfoLoader::Load());
		root["memory"] = JsonSerializer::Serialize(MemoryInfoLoader::Load());
		root["os"] = JsonSerializer::Serialize(OsInfoLoader::Load());
		root["topology"] = JsonSerializer::Serialize(topology);
		root["topology"]["fastestProcessor"] = timing.GetFastestProcessor();
		root["timing"] = JsonSerializer::Serialize(timing);
		root["benchmarkGroups"] = SerializeBenchmarkGroups(benchmarkPath);

		std::cout << root.dump();
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
