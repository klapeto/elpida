/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2022  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 10/12/2022.
//

#include <cstdlib>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <filesystem>

#include "Benchmarks/Image/Svg/SvgDocument.hpp"
#include "Benchmarks/Image/Svg/SvgUnits.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Platform/BenchmarkGroupModule.hpp"
#include "Elpida/Platform/TopologyLoader.hpp"
#include "Elpida/Platform/NumaAllocator.hpp"

#include "ArgumentsHelper.hpp"
#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Platform/MemoryInfoLoader.hpp"

using namespace Elpida;

static Vector<Ref<const ProcessingUnitNode>>
ValidateAndGetProcessingUnits(const Vector<unsigned int>& affinity, const TopologyInfo& topologyInfo)
{
	Vector<Ref<const ProcessingUnitNode>> returnVector;
	returnVector.reserve(affinity.size());

	auto& processors = topologyInfo.GetAllProcessingUnits();
	for (auto index : affinity)
	{
		bool found = false;
		for (auto& processor : processors)
		{
			if (processor.get().GetOsIndex().value() == index)
			{
				returnVector.emplace_back(processor.get());
				found = true;
				break;
			}
		}

		if (!found)
		{
			throw ElpidaException("No processor with id ", index, " was found");
		}
	}

	return returnVector;
}

static void
ValidateAndAssignConfiguration(const Vector<String>& configurationValues, Vector<TaskConfiguration>& taskConfigurations)
{
	if (configurationValues.size() != taskConfigurations.size())
	{
		throw ElpidaException("benchmark required ",
			taskConfigurations.size(),
			" configurations but were provided ",
			configurationValues.size());
	}

	for (Size i = 0; i < taskConfigurations.size(); ++i)
	{
		taskConfigurations[i].Parse(configurationValues[i]);
	}
}

int main(int argC, char** argV)
{
	std::fstream file("/home/klapeto/drawing.svg", std::ios::in | std::ios::binary);

	auto size = std::filesystem::file_size("/home/klapeto/drawing.svg");;

	auto data = new char[size];

	file.read(data, size);
	SvgDocument svg;

	svg.Load(data, size, Elpida::SvgUnits::PX, 96);

	return 0;
	try
	{
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

		BenchmarkGroupModule module(helper.GetModulePath());

		auto& benchmark = module
		.GetBenchmarkGroup()
			.GetBenchmarks()
			.at(helper.GetBenchmarkIndex());

		auto config = benchmark->GetRequiredConfiguration();
		ValidateAndAssignConfiguration(helper.GetConfigurationValues(), config);

		EnvironmentInfo environmentInfo(
			CpuInfoLoader::Load(),
			MemoryInfoLoader::Load(),
			OsInfoLoader::Load(),
			TopologyLoader::LoadTopology(),
			TimingInfo(NanoSeconds(helper.GetNowOverhead()),
				NanoSeconds(helper.GetLoopOverhead()),
				NanoSeconds(helper.GetVCallOverhead()),
				Seconds(0),
				0),
			std::make_unique<NumaAllocator>());

		auto targetProcessors = ValidateAndGetProcessingUnits(helper.GetAffinity(), environmentInfo.GetTopologyInfo());

		auto result = benchmark->Run(targetProcessors, config, environmentInfo);

		std::cout
			<< helper.GetResultFormatter().ConvertToString(result, *benchmark.get())
			<< std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}