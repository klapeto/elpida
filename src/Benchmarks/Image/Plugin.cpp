/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

/*
 * Plugin.cpp
 *
 *  Created on: 3 Ιουλ 2018
 *      Author: klapeto
 */

#include <Elpida/Utilities/Plugin/BenchmarksContainerPlugin.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/Calculators/Benchmark/AccumulativeScoreCalculator.hpp>
#include <Elpida/Engine/Calculators/Benchmark/AverageScoreCalculator.hpp>
#include <Elpida/Engine/Task/TaskBuilder.hpp>

#include <Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp>
#include "Benchmarks/Image/PngDecoding/PngDecodingSpecification.hpp"
#include "Benchmarks/Image/ConvertToFloat/ConvertToFloatSpecification.hpp"
#include "Benchmarks/Image/GrayscaleAverage/GrayscaleAverageSpecification.hpp"
#include "Benchmarks/Image/FloydSteinberg/FloydSteinbergSpecification.hpp"
#include "Benchmarks/Image/ConvertToUInt8/ConvertToUInt8Specification.hpp"
#include "Benchmarks/Image/PngEncoding/PngEncodingSpecification.hpp"
#include <Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp>

using namespace Elpida;

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL int32_t elpidaPluginAbiVersion()
{
	return ELPIDA_PLUGIN_ABI_VERSION;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL void elpidaDestroyPlugin(Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>* plugin)
{
	delete plugin;
}

static std::unique_ptr<Benchmark> createPngEncodingDecoding()
{
	auto benchmark =
		std::make_unique<Benchmark>("Png Encoding/Decoding", std::make_shared<AccumulativeScoreCalculator>());

	benchmark->AddTask<ReadFileSpecification>()
		.shouldBeCountedOnResults(false)
		.canBeMultiThreaded(false)
		.canBeDisabled(false)
		.withDefaultConfiguration(ReadFileSpecification::Settings::InputFilePath,
			ConfigurationType::FilePath("./Benchmark Material/shinobu.png"));

	benchmark->AddTask<PngDecodingSpecification>()
		.shouldBeCountedOnResults(true)
		.canBeDisabled(false)
		.canBeMultiThreaded(false);

	benchmark->AddTask<PngEncodingSpecification>()
		.shouldBeCountedOnResults(true)
		.canBeDisabled(false)
		.canBeMultiThreaded(false);

	return benchmark;
}

static std::unique_ptr<Benchmark> createFloydSteinberg()
{

	auto benchmark =
		std::make_unique<Benchmark>("Floyd Steinberg Dithering", std::make_unique<AccumulativeScoreCalculator>());

	benchmark->AddTask<ReadFileSpecification>()
		.shouldBeCountedOnResults(false)
		.canBeMultiThreaded(false)
		.canBeDisabled(false)
		.withDefaultConfiguration(ReadFileSpecification::Settings::InputFilePath,
			ConfigurationType::FilePath("/media/klapeto/Αρχεία/shinobu.png"));

	benchmark->AddTask<PngDecodingSpecification>()
		.shouldBeCountedOnResults(false)
		.canBeDisabled(false)
		.canBeMultiThreaded(false);


	benchmark->AddTask<ConvertToFloatSpecification>()
		.shouldBeCountedOnResults(false)
		.canBeDisabled(false)
		.canBeMultiThreaded(true);

	benchmark->AddTask<GrayscaleAverageSpecification>()
		.shouldBeCountedOnResults(false)
		.canBeDisabled(false)
		.canBeMultiThreaded(true);

	benchmark->AddTask<FloydSteinbergSpecification>()
		.shouldBeCountedOnResults(true)
		.withIterationsToRun(2)
		.canBeDisabled(false)
		.canBeMultiThreaded(false);

	benchmark->AddTask<ConvertToUInt8Specification>()
		.shouldBeCountedOnResults(false)
		.canBeDisabled(false)
		.canBeMultiThreaded(true);

	benchmark->AddTask<PngEncodingSpecification>()
		.shouldBeCountedOnResults(false)
		.canBeDisabled(false)
		.canBeMultiThreaded(false);

	benchmark->AddTask<WriteFileSpecification>()
		.shouldBeCountedOnResults(false)
		.canBeDisabled(true)
		.canBeMultiThreaded(false)
		.withDefaultConfiguration(WriteFileSpecification::Settings::OutputFilePath,
			ConfigurationType::FilePath("/media/klapeto/Αρχεία/shinobu_Floyd_Steinberg.png"));

	return benchmark;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL BenchmarksContainerPlugin<Benchmark>* elpidaCreatePlugin()
{
	auto plugin = new BenchmarksContainerPlugin<Benchmark>("Image Benchmarks");

	plugin->add(createFloydSteinberg());
	plugin->add(createPngEncodingDecoding());
	return plugin;
}
