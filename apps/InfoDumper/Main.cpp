/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
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

//
// Created by klapeto on 15/4/2024.
//

#include <cstdlib>
#include <iostream>
#include <string>
#include <filesystem>

#include "JsonSerializer.hpp"
#include "ArgumentsHelper.hpp"
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


static bool EndsWith(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

static json SerializeBenchmarkGroups(const std::filesystem::path& path, const std::string& suffix)
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
		if (!entry.is_directory()
			&& entry.is_regular_file()
			&& IsExecutable(entry)
			&& (suffix.empty() || EndsWith(entry.path().string(), suffix)))
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

	ArgumentsHelper helper;
	{
		std::string returnText;
		auto success = helper.ParseAndGetExitText(argC, argV, returnText);
		if (!returnText.empty())
		{
			std::cout << returnText << std::endl;
			return success ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}

	std::filesystem::path benchmarksPath;

	if (!helper.GetBenchmarksPath().empty())
	{
		std::string pathString = helper.GetBenchmarksPath().string();
		ValueUtilities::DeQuoteString(pathString);
		benchmarksPath = pathString;
	}
	else
	{
		benchmarksPath = OsUtilities::GetExecutableDirectory() / "Benchmarks";
	}

	std::string suffix = helper.GetBenchmarkSuffix();
	ValueUtilities::DeQuoteString(suffix);
	try
	{
		auto topology = TopologyLoader::LoadTopology();
		unsigned int highestCore = 0;

		if (!helper.IsNoThreadPinning())
		{
			topology.PinThreadToProcessor(0);
			auto& cores = topology.GetAllCores();

			Duration loopOverhead = Seconds(6546513);

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
		}

		auto timing = TimingCalculator::CalculateTiming();

		json root;
		root["cpu"] = JsonSerializer::Serialize(CpuInfoLoader::Load());
		root["memory"] = JsonSerializer::Serialize(MemoryInfoLoader::Load());
		root["os"] = JsonSerializer::Serialize(OsInfoLoader::Load());
		root["topology"] = JsonSerializer::Serialize(topology);
		root["topology"]["fastestProcessor"] = highestCore;
		root["timing"] = JsonSerializer::Serialize(timing);
		root["benchmarkGroups"] = SerializeBenchmarkGroups(benchmarksPath, suffix);

		std::cout << root.dump();
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}