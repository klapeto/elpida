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

#include "Elpida/Engine/BenchmarksContainerPlugin.hpp"
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/Calculators/Benchmark/AccumulativeScoreCalculator.hpp>
#include <Elpida/CommonTasks/ReadFile/ReadFileSpecification.hpp>
#include <Elpida/CommonTasks/WriteFile/WriteFileSpecification.hpp>
#include <Elpida/Engine/Task/TaskBuilder.hpp>
#include "Elpida/Engine/ServiceProvider.hpp"

using namespace Elpida;

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL int32_t elpidaPluginAbiVersion()
{
	return ELPIDA_PLUGIN_ABI_VERSION;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL void elpidaDestroyPlugin(BenchmarksContainerPlugin<Benchmark>* plugin)
{
	delete plugin;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL Elpida::BenchmarksContainerPlugin<Benchmark>*
elpidaCreatePlugin(const ServiceProvider* serviceProvider)
{
	auto plugin = new BenchmarksContainerPlugin<Benchmark>("Testing Benchmarks");
	auto& sharedStructures = serviceProvider->getSharedStructuresProvider();

	auto benchmark = std::make_unique<Benchmark>("Test Benchmark",
			BenchmarkScoreSpecification("Internets/s", Elpida::BenchmarkScoreComparison::Greater),
			sharedStructures.getAverageThroughputScoreCalculator());

	benchmark->AddTask(sharedStructures.getSharedTaskSpecification<ReadFileSpecification>())
			.shouldBeCountedOnResults()
			.canBeMultiThreaded(false)
			.canBeDisabled(false)
			.withDefaultConfiguration(ReadFileSpecification::Settings::InputFilePath,
					ConfigurationType::FilePath("/media/klapeto/Αρχεία/Isos/neon-user-20200326-1117.iso"));

	benchmark->AddTask(sharedStructures.getSharedTaskSpecification<WriteFileSpecification>())
			.shouldBeCountedOnResults()
			.canBeMultiThreaded(false)
			.canBeDisabled(false)
			.withDefaultConfiguration(WriteFileSpecification::Settings::OutputFilePath,
					ConfigurationType::FilePath("/media/klapeto/Αρχεία/Isos/TEST.iso"));;


	plugin->add(std::move(benchmark));

	return plugin;
}
