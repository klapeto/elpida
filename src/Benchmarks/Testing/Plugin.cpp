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

//
// Created by klapeto on 21/4/20.
//

#include <Elpida/Utilities/Plugin/BenchmarksContainerPlugin.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/AccumulativeScoreCalculator.hpp>
#include <Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp>
#include <Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp>
#include <Elpida/Engine/Task/TaskBuilder.hpp>

extern "C" ELPIDA_EXPORT Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>* createPlugin()
{
	using namespace Elpida;
	using Plugin = BenchmarksContainerPlugin<Elpida::Benchmark>;

	auto plugin = new Plugin();

	auto& readFile = (new TaskBuilder(*new ReadFileSpecification))
		->shouldBeCountedOnResults()
		.canBeMultiThreaded(false)
		.canBeDisabled(false)
		.withDefaultConfiguration(ReadFileSpecification::Settings::InputFilePath,
			ConfigurationType::FilePath("/media/klapeto/Αρχεία/Isos/neon-user-20200326-1117.iso"));

	auto& writeFile = (new TaskBuilder(*new WriteFileSpecification))
		->shouldBeCountedOnResults()
		.canBeMultiThreaded(false)
		.canBeDisabled(false)
		.withDefaultConfiguration(WriteFileSpecification::Settings::OutputFilePath,
			ConfigurationType::FilePath("/media/klapeto/Αρχεία/Isos/TEST.iso"));;

	auto benchmark = new Benchmark("Test Benchmark", {
		&readFile,
		&writeFile,
	}, new AccumulativeScoreCalculator());

	plugin->add(benchmark);

	return plugin;
}
