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

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <math.h>
#include <string_view>

#include "Benchmarks/Image/Svg/SvgDocument.hpp"
#include "Benchmarks/Image/Xml/XmlParser.hpp"
#include "Elpida/Core/AllocatorFactory.hpp"
#include "Elpida/Core/Config.hpp"
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
#include "Elpida/Platform/NumaAllocator.hpp"

#include "ArgumentsHelper.hpp"
#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Platform/MemoryInfoLoader.hpp"
#include "Benchmarks/Image/Xml/CharacterStream.hpp"

using namespace Elpida;

static Vector<Ref<const ProcessingUnitNode>>
ValidateAndGetProcessingUnits(const Vector<unsigned int>& affinity, const TopologyInfo& topologyInfo)
{
	Vector<Ref<const ProcessingUnitNode>> returnVector;
	returnVector.reserve(affinity.size());

	auto& processors = topologyInfo.GetAllProcessingUnits();
	for (auto index: affinity)
	{
		bool found = false;
		for (auto& processor: processors)
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
	std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
					  "<!-- Created with Inkscape (http://www.inkscape.org/) -->\n"
					  "\n"
					  "<svg\n"
					  "   width=\"74\"\n"
					  "   height=\"74\"\n"
					  "   preserveAspectRatio=\"xMidYMid slice\"\n"
					  "   viewBox=\"0 0 74 74\"\n"
					  "   version=\"1.1\"\n"
					  "   id=\"svg5\"\n"
					  "   inkscape:version=\"1.1.2 (0a00cf5339, 2022-02-04)\"\n"
					  "   sodipodi:docname=\"drawing.svg\"\n"
					  "   xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n"
					  "   xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"\n"
					  "   xmlns=\"http://www.w3.org/2000/svg\"\n"
					  "   xmlns:svg=\"http://www.w3.org/2000/svg\">\n"
					  "  <sodipodi:namedview\n"
					  "     id=\"namedview7\"\n"
					  "     pagecolor=\"#ffffff\"\n"
					  "     bordercolor=\"#666666\"\n"
					  "     borderopacity=\"1.0\"\n"
					  "     inkscape:pageshadow=\"2\"\n"
					  "     inkscape:pageopacity=\"0.0\"\n"
					  "     inkscape:pagecheckerboard=\"0\"\n"
					  "     inkscape:document-units=\"px\"\n"
					  "     showgrid=\"false\"\n"
					  "     units=\"px\"\n"
					  "     scale-x=\"1\"\n"
					  "     fit-margin-top=\"5\"\n"
					  "     fit-margin-left=\"5\"\n"
					  "     fit-margin-right=\"5\"\n"
					  "     fit-margin-bottom=\"5\"\n"
					  "     inkscape:zoom=\"9.0420721\"\n"
					  "     inkscape:cx=\"59.665527\"\n"
					  "     inkscape:cy=\"64.752857\"\n"
					  "     inkscape:window-width=\"3840\"\n"
					  "     inkscape:window-height=\"2056\"\n"
					  "     inkscape:window-x=\"3840\"\n"
					  "     inkscape:window-y=\"0\"\n"
					  "     inkscape:window-maximized=\"1\"\n"
					  "     inkscape:current-layer=\"layer1\" />\n"
					  "  <defs\n"
					  "     id=\"defs2\" >\n"
					  "    <linearGradient\n"
					  "       inkscape:collect=\"always\"\n"
					  "       xlink:href=\"#linearGradient1148\"\n"
					  "       id=\"linearGradient19685\"\n"
					  "       gradientUnits=\"userSpaceOnUse\"\n"
					  "       x1=\"318.09229\"\n"
					  "       y1=\"274.77167\"\n"
					  "       x2=\"319.75272\"\n"
					  "       y2=\"282.52206\"\n"
					  "       spreadMethod=\"pad\"\n"
					  "       gradientTransform=\"translate(523.9711,-70.819879)\" >"
					  "        <stop\n"
					  "         id=\"stop16172\"\n"
					  "         offset=\"0\"\n"
					  "         style=\"stop-color:#363765;stop-opacity:1;opacity:1\" />"
					  "        <stop\n"
					  "         id=\"stop16175\"\n"
					  "         offset=\"1\"\n"
					  "         style=\"stop-color:#9a9eef;stop-opacity:1;opacity:1\" />"
					  "      </linearGradient>"
					  "  </defs>"
					  "  <g\n"
					  "     inkscape:label=\"Layer 1\"\n"
					  "     inkscape:groupmode=\"layer\"\n"
					  "     id=\"layer1\"\n"
					  "     transform=\"translate(-51.583763,-70.752197)\">\n"
					  "    <circle\n"
					  "       style=\"fill:#b0aff2;fill-rule:evenodd;stroke-width:0.529167;stroke-linecap:round;stop-color:#000000;fill-opacity:1\"\n"
					  "       id=\"path846\"\n"
					  "       cx=\"88.583763\"\n"
					  "       cy=\"107.7522\"\n"
					  "       r=\"32\" />\n"
					  "  </g>\n"
					  "</svg>";


	auto x = "Ff";

	auto value = StrTol16(x);
	XmlParser parser;
	auto element = parser.Parse(xml.c_str(), xml.size());
	SvgDocument document(element);
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