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
#include <Elpida/Engine/AccumulativeScoreCalculator.hpp>
#include <Elpida/Engine/Task/TaskBuilder.hpp>

#include <Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp>
#include "Benchmarks/Image/PngDecoding/PngDecodingSpecification.hpp"
#include "Benchmarks/Image/ConvertToFloat/ConvertToFloatSpecification.hpp"
#include "Benchmarks/Image/GrayscaleAverage/GrayscaleAverageSpecification.hpp"
#include "Benchmarks/Image/FloydSteinberg/FloydSteinbergSpecification.hpp"
#include "Benchmarks/Image/ConvertToUInt8/ConvertToUInt8Specification.hpp"
#include "Benchmarks/Image/PngEncoding/PngEncodingSpecification.hpp"
#include <Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp>

static Elpida::Benchmark* createPngEncodingDecoding()
{
	using namespace Elpida;
	std::vector<TaskBuilder*> tasksBuilders;

	auto& readFile = (new TaskBuilder(*new ReadFileSpecification))
		->shouldBeCountedOnResults(false)
		.canBeMultiThreaded(false)
		.canBeDisabled(false)
		.withDefaultConfiguration(ReadFileSpecification::Settings::InputFilePath,
			ConfigurationType::FilePath("./Benchmark Material/shinobu.png"));

	auto& pngDecode = (new TaskBuilder(*new PngDecodingSpecification))
		->shouldBeCountedOnResults(true)
		.canBeDisabled(false)
		.canBeMultiThreaded(false);

	auto& pngEncoding =(new TaskBuilder(*new PngEncodingSpecification))
		->shouldBeCountedOnResults(true)
		.canBeDisabled(false)
		.canBeMultiThreaded(false);


	tasksBuilders.push_back(&readFile);
	tasksBuilders.push_back(&pngDecode);
	tasksBuilders.push_back(&pngEncoding);

	auto benchmark = new Elpida::Benchmark("Png Encoding/Decoding",
		std::move(tasksBuilders),
		new Elpida::AccumulativeScoreCalculator());
	return benchmark;

}

static Elpida::Benchmark* createFloydSteinberg()
{
	using namespace Elpida;
	std::vector<TaskBuilder*> tasksBuilders;

	auto& readFile = (new TaskBuilder(*new ReadFileSpecification))
		->shouldBeCountedOnResults(false)
		.canBeMultiThreaded(false)
		.canBeDisabled(false)
		.withDefaultConfiguration(ReadFileSpecification::Settings::InputFilePath,
			ConfigurationType::FilePath("./Benchmark Material/shinobu.png"));

	auto& pngDecode = (new TaskBuilder(*new PngDecodingSpecification))
		->shouldBeCountedOnResults(false)
		.canBeDisabled(false)
		.canBeMultiThreaded(false);


	auto& convertToFloat=(new TaskBuilder(*new ConvertToFloatSpecification))
		->shouldBeCountedOnResults(false)
		.canBeDisabled(false)
		.canBeMultiThreaded(true);

	auto& grayscale = (new TaskBuilder(*new GrayscaleAverageSpecification))
		->shouldBeCountedOnResults(false)
		.canBeDisabled(false)
		.canBeMultiThreaded(true);

	auto& floydSteinberg = (new TaskBuilder(*new FloydSteinbergSpecification))
		->shouldBeCountedOnResults(true)
		.canBeDisabled(false)
		.canBeMultiThreaded(false);

	auto& convertToUInt8 =(new TaskBuilder(*new ConvertToUInt8Specification))
		->shouldBeCountedOnResults(false)
		.canBeDisabled(false)
		.canBeMultiThreaded(true);

	auto& pngEncoding =(new TaskBuilder(*new PngEncodingSpecification))
		->shouldBeCountedOnResults(false)
		.canBeDisabled(false)
		.canBeMultiThreaded(false);


	auto& writeFile =(new TaskBuilder(*new WriteFileSpecification))
		->shouldBeCountedOnResults(false)
		.canBeDisabled(true)
		.canBeMultiThreaded(false)
		.withDefaultConfiguration(WriteFileSpecification::Settings::OutputFilePath,
			ConfigurationType::FilePath("./Benchmark Outputs/shinobu_Floyd_Steinberg.png"));


	tasksBuilders.push_back(&readFile);
	tasksBuilders.push_back(&pngDecode);
	tasksBuilders.push_back(&convertToFloat);
	tasksBuilders.push_back(&grayscale);
	tasksBuilders.push_back(&floydSteinberg);
	tasksBuilders.push_back(&convertToUInt8);
	tasksBuilders.push_back(&pngEncoding);
	tasksBuilders.push_back(&writeFile);

	auto benchmark = new Elpida::Benchmark("Floyd Steinberg Dithering",
		std::move(tasksBuilders),
		new Elpida::AccumulativeScoreCalculator());
	return benchmark;

}

extern "C" ELPIDA_EXPORT Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>* createPlugin()
{
	using namespace Elpida;
	using Plugin = BenchmarksContainerPlugin<Elpida::Benchmark>;

	auto plugin = new Plugin("Image Benchmarks");

	plugin->add(createFloydSteinberg());
	plugin->add(createPngEncodingDecoding());
	return plugin;
}
