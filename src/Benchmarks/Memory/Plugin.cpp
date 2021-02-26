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


std::unique_ptr<Benchmark> createMemoryReadBandwidth(
	const std::shared_ptr<BenchmarkScoreCalculator>& benchmarkScoreCalculator,
	const std::shared_ptr<TaskResultCalculator>& taskResultCalculator);
std::unique_ptr<Benchmark> createMemoryReadLatency(
	const std::shared_ptr<BenchmarkScoreCalculator>& benchmarkScoreCalculator,
	const std::shared_ptr<TaskResultCalculator>& taskResultCalculator);

std::unique_ptr<Benchmark> createMemoryReadLatencyFast(
	const std::shared_ptr<BenchmarkScoreCalculator>& benchmarkScoreCalculator,
	const std::shared_ptr<TaskResultCalculator>& taskResultCalculator);


extern "C" ELPIDA_EXPORT ELPIDA_STDCALL int32_t elpidaPluginAbiVersion()
{
	return ELPIDA_PLUGIN_ABI_VERSION;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL void elpidaDestroyPlugin(Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>* plugin)
{
	delete plugin;
}

extern "C" ELPIDA_EXPORT ELPIDA_STDCALL Elpida::BenchmarksContainerPlugin<Elpida::Benchmark>* elpidaCreatePlugin(const ServiceProvider* serviceProvider)
{
	using Plugin = BenchmarksContainerPlugin<Benchmark>;

	auto plugin = new Plugin("Memory Benchmarks");

	auto averageTimeScoreCalculator = std::make_shared<AverageScoreCalculator>("s");
	auto averageTaskCalculator = std::make_shared<ExclusiveHarmonicMeanTaskResultCalculator>(1);
	auto averageBandwidthScoreCalculator = std::make_shared<AccumulativeScoreCalculator>("B", ResultType::Throughput);

	plugin->add(createMemoryReadBandwidth(averageBandwidthScoreCalculator, averageTaskCalculator));
	plugin->add(createMemoryReadLatency(averageTimeScoreCalculator, averageTaskCalculator));
	plugin->add(createMemoryReadLatencyFast(averageTimeScoreCalculator, averageTaskCalculator));

	return plugin;
}

std::unique_ptr<Benchmark> createMemoryReadLatency(
	const std::shared_ptr<BenchmarkScoreCalculator>& benchmarkScoreCalculator,
	const std::shared_ptr<TaskResultCalculator>& taskResultCalculator)
{
	auto benchmark = std::make_unique<Benchmark>("Memory Read Latency (Slow)", benchmarkScoreCalculator);

	auto readLatencySpec = std::make_shared<MemoryLatencySpecification>("Memory");

	for (auto size : WorkingSetSizes::Values)
	{
		benchmark->AddTask(readLatencySpec)
			.shouldBeCountedOnResults(true)
			.withIterationsToRun(50)
			.withTaskResultCalculator(taskResultCalculator)
			.canBeMultiThreaded(false)
			.canBeDisabled(false)
			.withFixedConfiguration(MemoryLatencySpecification::Settings::MemorySizeCalculator,
				MemoryLatencySpecification::Settings::MemorySizeCalculatorT([size](const auto&)
				{
					return size;
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

std::unique_ptr<Benchmark> createMemoryReadLatencyFast(
	const std::shared_ptr<BenchmarkScoreCalculator>& benchmarkScoreCalculator,
	const std::shared_ptr<TaskResultCalculator>& taskResultCalculator)
{
	auto benchmark = std::make_unique<Benchmark>("Memory Read Latency (Fast)", benchmarkScoreCalculator);


	benchmark->AddTask(std::make_shared<MemoryLatencySpecification>("L1D"))
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
							return p.getType() == ProcessorNodeType::L1DCache;
						}) / 2;
				}));

	benchmark->AddTask(std::make_shared<MemoryLatencySpecification>("L2D"))
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
							return p.getType() == ProcessorNodeType::L2DCache;
						}) / 2;
				}));


	benchmark->AddTask(std::make_shared<MemoryLatencySpecification>("L3D"))
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
					return calculateSizeForTargetProcessorType(x, [](const ProcessorNode& p)
					{
						return p.getType() == ProcessorNodeType::L3DCache;
					}) / 2;
				}));


	benchmark->AddTask(std::make_shared<MemoryLatencySpecification>("DRAM"))
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

std::unique_ptr<Benchmark> createMemoryReadBandwidth(const std::shared_ptr<BenchmarkScoreCalculator>& benchmarkScoreCalculator,
	const std::shared_ptr<TaskResultCalculator>& taskResultCalculator)
{
	auto benchmark = std::make_unique<Benchmark>("Memory Read Bandwidth", benchmarkScoreCalculator);

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