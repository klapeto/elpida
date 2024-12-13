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
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Platform/Process.hpp"
#include "Elpida/Platform/AsyncPipeReader.hpp"

using namespace nlohmann;
using namespace Elpida;

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

int main(int argC, char** argV)
{
	OsUtilities::ConvertArgumentsToUTF8(argC, argV);

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

		topology.PinThreadToProcessor(0);
		auto& cores = topology.GetAllCores();

		Duration loopOverhead = Seconds(6546513);
		unsigned int highestCore = 0;

		for (auto& core : cores)
		{
			auto& pu = core.get().GetChildren().front();

			Duration overhead;

			topology.PinThreadToProcessor(pu.get()->GetOsIndex().value());
			overhead = TimingCalculator::CalculateLoopOverheadFast();

			if (overhead < loopOverhead)
			{
				highestCore = pu->GetOsIndex().value();
				loopOverhead = overhead;
			}
		}

		topology.PinThreadToProcessor(highestCore);

		auto timing = TimingCalculator::CalculateTiming();

		json root;
		root["cpu"] = JsonSerializer::Serialize(CpuInfoLoader::Load());
		root["memory"] = JsonSerializer::Serialize(MemoryInfoLoader::Load());
		root["os"] = JsonSerializer::Serialize(OsInfoLoader::Load());
		root["topology"] = JsonSerializer::Serialize(topology);
		root["topology"]["fastestProcessor"] = highestCore;
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