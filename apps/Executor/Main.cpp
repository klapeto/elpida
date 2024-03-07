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

#include <stdexcept>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <math.h>
#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgEllipseEquation.hpp>
#include <Elpida/Svg/SvgRasterizer.hpp>
#include <Elpida/Xml/XmlParser.hpp>

#include "Elpida/Core/AllocatorFactory.hpp"
#include "Elpida/Core/DefaultAllocatorFactory.hpp"
#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Platform/BenchmarkGroupModule.hpp"
#include "Elpida/Platform/NumaAllocatorFactory.hpp"
#include "Elpida/Platform/TopologyLoader.hpp"

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

#include <fstream>
#include <png.h>

#include "Elpida/Svg/SvgColorConverter.hpp"


int main(int argC, char** argV)
{
	std::ifstream file("/home/klapeto/σχεδίαση.svg", std::ifstream::binary | std::ifstream::in);


	file.seekg(0,std::ifstream::end);
	auto  size = file.tellg();
	file.seekg(0, std::ifstream::beg);

	auto inputData = std::unique_ptr<char[]>(new char[size]);

	file.read(inputData.get(), size);


	auto element = XmlParser::Parse(inputData.get(), size);

	SvgDocument svgDocument(element);

	std::size_t width = svgDocument.GetActualWidth();
	std::size_t height = svgDocument.GetActualHeight();

	SvgRasterizer rasterizer;
	auto backDrop = rasterizer.Rasterize(svgDocument);

	auto imageData = SvgColorConverter::Convert<unsigned char>(backDrop.GetColorData());
	{
		png_image image{};
		image.version = PNG_IMAGE_VERSION;

		std::size_t outputSize;

		image.width = width;
		image.height = height;
		image.format = PNG_FORMAT_RGBA;

		if (png_image_write_to_memory(&image, nullptr, &outputSize, 0, imageData.get(), 0, nullptr))
		{
			auto outputData = std::unique_ptr<char[]>(new char[outputSize]);

			if (!png_image_write_to_memory(&image,
						outputData.get(),
						&outputSize,
						0,
					imageData.get(),
						0,
						nullptr))
			{
				throw ElpidaException("Failed to encode image: ", image.message);
			}

			std::ofstream outputFile("/home/klapeto/σχεδίαση.out.png", std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);

			outputFile.write(outputData.get(), outputSize);
		}
		else
		{
			throw ElpidaException("Failed to encode image: ", image.message);
		}
	}



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
			helper.GetNumaAware()
				? UniquePtr<AllocatorFactory>(new NumaAllocatorFactory())
				: UniquePtr<AllocatorFactory>(new DefaultAllocatorFactory()));

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
