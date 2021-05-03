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
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#include <Elpida/Utilities/Plugin/BenchmarksContainerPlugin.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Engine/Calculators/Benchmark/AccumulativeScoreCalculator.hpp>
#include <Elpida/CommonTasks/AllocateMemory/AllocateMemorySpecification.hpp>
#include <Elpida/Engine/Task/TaskBuilder.hpp>
#include <Elpida/Engine/Calculators/Benchmark/AverageScoreCalculator.hpp>
#include <Elpida/Engine/Calculators/Task/ExclusiveHarmonicMeanTaskResultCalculator.hpp>
#include <Elpida/SystemInfo/ProcessorNode.hpp>
#include <Elpida/SystemInfo/CpuInfo.hpp>
#include <Elpida/ServiceProvider.hpp>
#include <Elpida/Engine/Calculators/TaskResultCalculator.hpp>
#include <Elpida/Utilities/Statistics.hpp>

#include "Benchmarks/Memory/Bandwidth/MemoryReadBandwidthSpecification.hpp"
#include "Benchmarks/Memory/Latency/MemoryLatencySpecification.hpp"

#include "Benchmarks/Memory/WorkingSetSizes.hpp"

#include <memory>

using namespace Elpida;


std::unique_ptr<Benchmark> createOverallMemoryLatency(
		const BenchmarkScoreSpecification& scoreSpecification,
		const SharedStructuresProvider& structuresProvider,
		const std::shared_ptr<TaskResultCalculator>& taskResultCalculator)
{

	auto benchmark = std::make_unique<Benchmark>("Overall memory latency",
			scoreSpecification,
			structuresProvider.getAverageBenchmarkScoreCalculator(),
			"2b3bb7da-0226-4a96-a473-1dc67ee37422");

	for (const auto&[uuid, value] : WorkingSetSizes::BenchmarkValues)
	{
		auto memorySize = value;
		benchmark->AddTask(std::make_shared<MemoryLatencySpecification>(
						"Memory Latency [" + Vu::getValueScaleStringIEC(memorySize) + "B]", uuid))
				.shouldBeCountedOnResults(true)
				.withIterationsToRun(50)
				.withTaskResultCalculator(taskResultCalculator)
				.canBeMultiThreaded(false)
				.canBeDisabled(false)
				.withFixedConfiguration(MemoryLatencySpecification::Settings::MemorySizeCalculator,
						MemoryLatencySpecification::Settings::MemorySizeCalculatorT(
								[memorySize](const auto&)
								{
									return memorySize;
								}));
	}

	return benchmark;
}

static bool IsLocalMemory(const ProcessorNode& processorToRun)
{
	switch (processorToRun.getType())
	{
	case ProcessorNodeType::ExecutionUnit:
	case ProcessorNodeType::Core:
	case ProcessorNodeType::L1DCache:
	case ProcessorNodeType::L1ICache:
	case ProcessorNodeType::L2DCache:
	case ProcessorNodeType::L2ICache:
	case ProcessorNodeType::L3DCache:
	case ProcessorNodeType::L3ICache:
	case ProcessorNodeType::L4Cache:
	case ProcessorNodeType::L5Cache:
		return true;
	default:
		return false;
	}
}

static bool hasAncestor(const ProcessorNode& processorNode, ProcessorNodeType type)
{
	auto currentProcessor = processorNode.getParent();

	while (currentProcessor.has_value())
	{
		if (currentProcessor->get().getType() == type)
		{
			return true;
		}
		currentProcessor = currentProcessor->get().getParent();
	}
	return false;
}

static size_t calculateSizeForMemory(const ProcessorNode& processorNode)
{
	auto currentProcessor = processorNode.getParent();

	do
	{
		auto parent = currentProcessor->get().getParent();
		if (parent.has_value() && !IsLocalMemory(parent->get()))
		{
			break;
		}
		currentProcessor = parent;
	} while (currentProcessor.has_value());

	return currentProcessor.has_value() ? currentProcessor->get().getValue() : processorNode.getValue();
}

template<typename TCallable>
static size_t calculateSizeForTargetProcessorType(const ProcessorNode& processorNode, TCallable nodeValidator)
{
	auto currentProcessor = processorNode.getParent();

	while (currentProcessor.has_value())
	{
		if (nodeValidator(currentProcessor->get()))
		{
			break;
		}
		currentProcessor = currentProcessor->get().getParent();
	}

	return currentProcessor.has_value() ? currentProcessor->get().getValue() : processorNode.getValue();
}

std::unique_ptr<Benchmark> createL1DLatency(
		const BenchmarkScoreSpecification& scoreSpecification,
		const SharedStructuresProvider& structuresProvider,
		const std::shared_ptr<TaskResultCalculator>& taskResultCalculator)
{
	auto benchmark = std::make_unique<Benchmark>("L1D Cache Latency",
			scoreSpecification,
			structuresProvider.getAverageBenchmarkScoreCalculator(),
			"ebe54867-eb7e-4848-8bd0-5286b77fdcdd");

	benchmark->AddTask(std::make_shared<MemoryLatencySpecification>("L1D Cache Latency",
					"7b401200-1e5a-4e3e-96b4-59ece5c23e3b"))
			.shouldBeCountedOnResults(true)
			.withIterationsToRun(50)
			.withTaskResultCalculator(taskResultCalculator)
			.canBeMultiThreaded(false)
			.canBeDisabled(false)
			.withConditionalExecution([](const auto&, const ProcessorNode& processor, const auto&)
			{
				return hasAncestor(processor, ProcessorNodeType::L1DCache);
			})
			.withFixedConfiguration(MemoryLatencySpecification::Settings::MemorySizeCalculator,
					MemoryLatencySpecification::Settings::MemorySizeCalculatorT(
							[](const auto& x)
							{
								return calculateSizeForTargetProcessorType(x,
										[](const ProcessorNode& p)
										{
											return p.getType() ==
												   ProcessorNodeType::L1DCache;
										}) / 2;
							}));

	return benchmark;
}

std::unique_ptr<Benchmark> createL2DLatency(
		const BenchmarkScoreSpecification& scoreSpecification,
		const SharedStructuresProvider& structuresProvider,
		const std::shared_ptr<TaskResultCalculator>& taskResultCalculator)
{
	auto benchmark = std::make_unique<Benchmark>("L2D Cache Latency",
			scoreSpecification,
			structuresProvider.getAverageBenchmarkScoreCalculator(),
			"3ad2c346-926c-4f2f-b57a-0a046757b7eb");

	benchmark->AddTask(std::make_shared<MemoryLatencySpecification>("L2D Cache Latency",
					"bf049f8d-3b84-4e98-badb-bf8cf0ca0249"))
			.shouldBeCountedOnResults(true)
			.withIterationsToRun(50)
			.withTaskResultCalculator(taskResultCalculator)
			.canBeMultiThreaded(false)
			.canBeDisabled(false)
			.withConditionalExecution([](const auto&, const ProcessorNode& processor, const auto&)
			{
				return hasAncestor(processor, ProcessorNodeType::L2DCache);
			})
			.withFixedConfiguration(MemoryLatencySpecification::Settings::MemorySizeCalculator,
					MemoryLatencySpecification::Settings::MemorySizeCalculatorT(
							[](const auto& x)
							{
								return calculateSizeForTargetProcessorType(x,
										[](const ProcessorNode& p)
										{
											return p.getType() ==
												   ProcessorNodeType::L2DCache;
										}) / 2;
							}));

	return benchmark;
}

std::unique_ptr<Benchmark> createL3DLatency(
		const BenchmarkScoreSpecification& scoreSpecification,
		const SharedStructuresProvider& structuresProvider,
		const std::shared_ptr<TaskResultCalculator>& taskResultCalculator)
{
	auto benchmark = std::make_unique<Benchmark>("L3D Cache Latency",
			scoreSpecification,
			structuresProvider.getAverageBenchmarkScoreCalculator(),
			"4543c724-b17b-4425-8104-79d819825621");

	benchmark->AddTask(std::make_shared<MemoryLatencySpecification>("L3D Cache Latency",
					"4552ef8e-c4f8-44d6-8a49-7d24eb8f5df1"))
			.shouldBeCountedOnResults(true)
			.withIterationsToRun(50)
			.withTaskResultCalculator(taskResultCalculator)
			.canBeMultiThreaded(false)
			.canBeDisabled(false)
			.withConditionalExecution([](const auto&, const ProcessorNode& processor, const auto&)
			{
				return hasAncestor(processor, ProcessorNodeType::L3DCache);
			})
			.withFixedConfiguration(MemoryLatencySpecification::Settings::MemorySizeCalculator,
					MemoryLatencySpecification::Settings::MemorySizeCalculatorT(
							[](const auto& x)
							{
								return calculateSizeForTargetProcessorType(x,
										[](const ProcessorNode& p)
										{
											return p.getType() ==
												   ProcessorNodeType::L3DCache;
										}) / 2;
							}));

	return benchmark;
}


std::unique_ptr<Benchmark> createDramLatency(
		const BenchmarkScoreSpecification& scoreSpecification,
		const SharedStructuresProvider& structuresProvider,
		const std::shared_ptr<TaskResultCalculator>& taskResultCalculator)
{
	auto benchmark = std::make_unique<Benchmark>("DRAM Latency",
			scoreSpecification,
			structuresProvider.getAverageBenchmarkScoreCalculator(),
			"7626cdc2-6f7b-4c34-8cf9-b6e3fcb4c735");

	benchmark->AddTask(std::make_shared<MemoryLatencySpecification>("DRAM Latency",
					"7b184be3-9915-4529-8e02-c5db88191c7f"))
			.shouldBeCountedOnResults(true)
			.withIterationsToRun(50)
			.withTaskResultCalculator(taskResultCalculator)
			.canBeMultiThreaded(false)
			.canBeDisabled(false)
			.withFixedConfiguration(MemoryLatencySpecification::Settings::MemorySizeCalculator,
					MemoryLatencySpecification::Settings::MemorySizeCalculatorT(
							[](const auto& x)
							{
								return calculateSizeForMemory(x) * 2;
							}));

	return benchmark;
}

std::unique_ptr<Benchmark>
createMemoryReadBandwidth(const SharedStructuresProvider& structuresProvider,
		const std::shared_ptr<TaskResultCalculator>& taskResultCalculator)
{
	auto benchmark = std::make_unique<Benchmark>("Memory Read Bandwidth",
			BenchmarkScoreSpecification("B/s", Elpida::BenchmarkScoreComparison::Greater),
			structuresProvider.getAverageThroughputScoreCalculator(),
			"7e9e2eb8-0ea4-489a-b2f3-4cb133c04bc8");

	benchmark->AddTask<MemoryReadBandwidthSpecification>()
			.shouldBeCountedOnResults(true)
			.withTaskResultCalculator(taskResultCalculator)
			.canBeMultiThreaded(true)
			.canBeDisabled(false)
			.withFixedConfiguration(MemoryReadBandwidthSpecification::Settings::MemorySizeCalculator,
					MemoryReadBandwidthSpecification::Settings::MemorySizeCalculatorT(
							[](const auto& x)
							{
								return calculateSizeForMemory(x) * 2;
							}));

	return benchmark;
}


extern "C" ELPIDA_EXPORT ELPIDA_STDCALL int32_t elpidaPluginAbiVersion()
{
	return ELPIDA_PLUGIN_ABI_VERSION;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL void
elpidaDestroyPlugin(Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>* plugin)
{
	delete plugin;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>*
elpidaCreatePlugin(const ServiceProvider* serviceProvider)
{
	using Plugin = BenchmarksContainerPlugin<Benchmark>;

	auto plugin = new Plugin("Memory Benchmarks");

	auto averageTaskCalculator = std::make_shared<ExclusiveHarmonicMeanTaskResultCalculator>(1);

	BenchmarkScoreSpecification latencySpecification("s", Elpida::BenchmarkScoreComparison::Lower);

	auto structuresProvider = serviceProvider->getSharedStructuresProvider();

	plugin->add(createMemoryReadBandwidth(structuresProvider, averageTaskCalculator));
	plugin->add(createOverallMemoryLatency(latencySpecification, structuresProvider, averageTaskCalculator));
	plugin->add(createL1DLatency(latencySpecification, structuresProvider, averageTaskCalculator));
	plugin->add(createL2DLatency(latencySpecification, structuresProvider, averageTaskCalculator));
	plugin->add(createL3DLatency(latencySpecification, structuresProvider, averageTaskCalculator));
	plugin->add(createDramLatency(latencySpecification, structuresProvider, averageTaskCalculator));

	return plugin;
}