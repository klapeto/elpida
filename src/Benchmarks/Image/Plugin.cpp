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

#include <Elpida/Engine/BenchmarksContainerPlugin.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/Calculators/Benchmark/AccumulativeScoreCalculator.hpp>
#include <Elpida/Engine/Task/TaskBuilder.hpp>
#include <Elpida/Engine/ServiceProvider.hpp>

#include <Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp>
#include <Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp>
#include <utility>

#include "Benchmarks/Image/PngDecoding/PngDecodingSpecification.hpp"
#include "Benchmarks/Image/ConvertToFloat/ConvertToFloatSpecification.hpp"
#include "Benchmarks/Image/GrayscaleAverage/GrayscaleAverageSpecification.hpp"
#include "Benchmarks/Image/FloydSteinberg/FloydSteinbergSpecification.hpp"
#include "Benchmarks/Image/ConvertToUInt8/ConvertToUInt8Specification.hpp"
#include "Benchmarks/Image/PngEncoding/PngEncodingSpecification.hpp"


using namespace Elpida;

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL int32_t elpidaPluginAbiVersion()
{
	return ELPIDA_PLUGIN_ABI_VERSION;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL void
elpidaDestroyPlugin(Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>* plugin)
{
	delete plugin;
}

static std::unique_ptr<Benchmark> createPngEncoding(
		SharedStructuresProvider& structuresProvider, std::shared_ptr<ReadFileSpecification> readFileSpec)
{
	auto benchmark =
			std::make_unique<Benchmark>("Png Encoding",
					BenchmarkScoreSpecification("Pixels/s", Elpida::BenchmarkScoreComparison::Greater),
					structuresProvider.getAverageThroughputScoreCalculator(),
					"bfec734b-8db7-44a1-b011-b48eac29005c");

	benchmark->AddTask(std::move(readFileSpec))
			.shouldBeCountedOnResults(false)
			.canBeMultiThreaded(false)
			.canBeDisabled(false)
			.withDefaultConfiguration(ReadFileSpecification::Settings::InputFilePath,
					ConfigurationType::FilePath("./Benchmark Material/shinobu.png"));

	benchmark->AddTask<PngDecodingSpecification>()
			.shouldBeCountedOnResults(false)
			.canBeDisabled(false)
			.canBeMultiThreaded(false);

	benchmark->AddTask<PngEncodingSpecification>()
			.shouldBeCountedOnResults(true)
			.canBeDisabled(false)
			.canBeMultiThreaded(false);

	return benchmark;
}

static std::unique_ptr<Benchmark> createPngDecoding(
		SharedStructuresProvider& structuresProvider, std::shared_ptr<ReadFileSpecification> readFileSpec)
{
	auto benchmark =
			std::make_unique<Benchmark>("Png Decoding",
					BenchmarkScoreSpecification("Pixels/s", Elpida::BenchmarkScoreComparison::Greater),
					structuresProvider.getAverageThroughputScoreCalculator(),
					"a4507ca2-99bf-4d19-8bdb-236420f15246");

	benchmark->AddTask(std::move(readFileSpec))
			.shouldBeCountedOnResults(false)
			.canBeMultiThreaded(false)
			.canBeDisabled(false)
			.withDefaultConfiguration(ReadFileSpecification::Settings::InputFilePath,
					ConfigurationType::FilePath("./Benchmark Material/shinobu.png"));

	benchmark->AddTask<PngDecodingSpecification>()
			.shouldBeCountedOnResults(true)
			.canBeDisabled(false)
			.canBeMultiThreaded(false);

	return benchmark;
}

static std::unique_ptr<Benchmark> createFloydSteinberg(SharedStructuresProvider& structuresProvider, std::shared_ptr<ReadFileSpecification> readFileSpec, std::shared_ptr<WriteFileSpecification> writeFileSpec)
{
	auto benchmark =
			std::make_unique<Benchmark>("Floyd Steinberg Dithering",
					BenchmarkScoreSpecification("Pixels/s", Elpida::BenchmarkScoreComparison::Greater),
					structuresProvider.getAverageThroughputScoreCalculator(),
					"ec02cf8f-8820-41cc-9f3b-69473d44a261");

	benchmark->AddTask(std::move(readFileSpec))
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

	benchmark->AddTask(std::move(writeFileSpec))
			.shouldBeCountedOnResults(false)
			.canBeDisabled(true)
			.canBeMultiThreaded(false)
			.withDefaultConfiguration(WriteFileSpecification::Settings::OutputFilePath,
					ConfigurationType::FilePath("/media/klapeto/Αρχεία/shinobu_Floyd_Steinberg.png"));

	return benchmark;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL BenchmarksContainerPlugin<Benchmark>*
elpidaCreatePlugin(const ServiceProvider* serviceProvider)
{
	auto plugin = new BenchmarksContainerPlugin<Benchmark>("Image Benchmarks");

	auto structuresProvider = serviceProvider->getSharedStructuresProvider();

    auto readFileSpec = std::make_shared<ReadFileSpecification>();
    auto writeFileSpec = std::make_shared<WriteFileSpecification>();

	plugin->add(createFloydSteinberg(structuresProvider, readFileSpec, writeFileSpec));
	plugin->add(createPngEncoding(structuresProvider, readFileSpec));
	plugin->add(createPngDecoding(structuresProvider, readFileSpec));
	return plugin;
}
