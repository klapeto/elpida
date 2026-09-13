//  Copyright (c) 2026  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 29/6/26.
//

#include "InProcessBenchmarkExecutionService.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "ElpidaInstance.hpp"
#include "Benchmarks/Compilation/ClangCompilationBenchmark.hpp"
#include "Benchmarks/Compression/ZlibCompressionBenchmark.hpp"
#include "Benchmarks/Compression/ZlibDecompressionBenchmark.hpp"
#include "Benchmarks/Encryption/AESDecryptionBenchmark.hpp"
#include "Benchmarks/Encryption/AESEncryptionBenchmark.hpp"
#include "Benchmarks/Encryption/RSADecryptionBenchmark.hpp"
#include "Benchmarks/Encryption/RSAEncryptionBenchmark.hpp"
#include "Benchmarks/Image/PngEncodingBenchmark.hpp"
#include "Benchmarks/Image/RayTracingBenchmark.hpp"
#include "Benchmarks/Image/SvgParseBenchmark.hpp"
#include "Benchmarks/Image/SvgRasterization2Benchmark.hpp"
#include "Benchmarks/Image/SvgRasterizationBenchmark.hpp"
#include "Benchmarks/Image/SvgRasterizationFromFileBenchmark.hpp"
#include "Benchmarks/Math/CalculateFFTInPlaceBenchmark.hpp"
#include "Benchmarks/Math/DynamicMatrixMultiplicationBenchmark.hpp"
#include "Benchmarks/Math/MatrixInverse4Benchmark.hpp"
#include "Benchmarks/Math/MatrixMultiplication32Benchmark.hpp"
#include "Benchmarks/Math/NBodyBenchmark.hpp"
#include "Benchmarks/Memory/MemoryLatencyBenchmark.hpp"
#include "Benchmarks/Memory/MemoryReadBandwidthBenchmark.hpp"
#include "Benchmarks/StdLib/CeilBenchmark.hpp"
#include "Benchmarks/StdLib/FloorBenchmark.hpp"
#include "Benchmarks/StdLib/MallocBenchmark.hpp"
#include "Benchmarks/StdLib/MemsetBenchmark.hpp"
#include "Benchmarks/StdLib/RegexBenchmark.hpp"
#include "Benchmarks/StdLib/MemchrBenchmark.hpp"
#include "Benchmarks/StdLib/SqrtBenchmark.hpp"
#include "Benchmarks/StdLib/StrtodBenchmark.hpp"
#include "Benchmarks/StdLib/StrtoulBenchmark.hpp"
#include "Benchmarks/Web/Base64DecodeBenchmark.hpp"
#include "Benchmarks/Web/Base64EncodeBenchmark.hpp"
#include "Benchmarks/Web/JsonParseBenchmark.hpp"
#include "Benchmarks/Web/XmlParseBenchmark.hpp"
#include "Benchmarks/Web/RegexBenchmark.hpp"
#include "Elpida/Core/DefaultAllocatorFactory.hpp"
#include "Models/Benchmark/BenchmarkModel.hpp"

namespace Elpida
{
	class MemchrBenchmark;
}

using namespace Elpida;
using namespace Elpida::Application;
using namespace Elpida::Benchmarks;

BenchmarkResultModel InProcessBenchmarkExecutionService::Execute(
	const BenchmarkModel& benchmarkModel,
	const std::vector<std::size_t>& affinity,
	double nowOverheadSeconds,
	double loopOverheadSeconds,
	bool numaAware,
	bool pinThreads,Application::ConcurrencyMode concurrencyMode,
	double minimumMicroTaskDuration)
{

	std::vector<TaskConfiguration> config;

	for (auto& thisConfig: benchmarkModel.GetConfigurations())
	{
		config.emplace_back(thisConfig.GetName(), static_cast<Elpida::ConfigurationType>(thisConfig.GetType()), thisConfig.GetValue());
	}

	auto context = BenchmarkRunContext(
		_instance->environmentInfo.GetTopologyInfo().GetAllProcessingUnits(),
		config,
		Elpida::UniquePtr<AllocatorFactory>(new DefaultAllocatorFactory()),
		_instance->environmentInfo,
		static_cast<Elpida::ConcurrencyMode>(concurrencyMode),
		false,
		Seconds(minimumMicroTaskDuration));

	auto itr = _benchmarks.find(benchmarkModel.GetName());

	if (itr == _benchmarks.end())
	{
		throw ElpidaException("Benchmark not found: " + benchmarkModel.GetName());
	}

	auto benchmark = itr->second.get();

	auto result = benchmark->Run(context);

	std::unordered_map<std::string, std::string> outputConfig;


	for (auto& thisConfig: benchmarkModel.GetConfigurations())
	{
		outputConfig[thisConfig.GetName()] = thisConfig.GetValue();
	}

	return BenchmarkResultModel(benchmarkModel, result.GetResult(), std::move(outputConfig));
}

void InProcessBenchmarkExecutionService::StopCurrentExecution()
{
}


InProcessBenchmarkExecutionService::InProcessBenchmarkExecutionService()
	: _instance(nullptr)
{
	AddBenchmark<Compilation::ClangCompilationBenchmark>();
	AddBenchmark<Compression::ZlibCompressionBenchmark>();
	AddBenchmark<Compression::ZlibDecompressionBenchmark>();
	AddBenchmark<Compression::ZlibDecompressionBenchmark>();
	AddBenchmark<Encryption::RSAEncryptionBenchmark>();
	AddBenchmark<Encryption::RSADecryptionBenchmark>();
	AddBenchmark<Encryption::AESEncryptionBenchmark>();
	AddBenchmark<Encryption::AESDecryptionBenchmark>();
	AddBenchmark<Image::PngEncodingBenchmark>();
	AddBenchmark<Image::SvgParseBenchmark>();
	AddBenchmark<Image::SvgRasterizationBenchmark>();
	AddBenchmark<Image::SvgRasterization2Benchmark>();
	AddBenchmark<Image::SvgRasterizationFromFileBenchmark>();
	AddBenchmark<Image::RayTracingBenchmark>();
	AddBenchmark<Math::CalculateFFTInPlaceBenchmark>();
	AddBenchmark<Math::DynamicMatrixMultiplicationBenchmark>();
	AddBenchmark<Math::MatrixMultiplication32Benchmark>();
	AddBenchmark<Math::MatrixInverse4Benchmark>();
	AddBenchmark<Math::NBodyBenchmark>();
	AddBenchmark<Benchmarks::Memory::MemoryReadBandwidthBenchmark>();
	AddBenchmark<Benchmarks::Memory::MemoryLatencyBenchmark>();
	AddBenchmark<StdLib::MallocBenchmark>();
	AddBenchmark<StdLib::StrtodBenchmark>();
	AddBenchmark<StdLib::StrtoulBenchmark>();
	AddBenchmark<StdLib::MemsetBenchmark>();
	AddBenchmark<StdLib::MemchrBenchmark>();
	AddBenchmark<StdLib::SqrtBenchmark>();
	AddBenchmark<StdLib::CeilBenchmark>();
	AddBenchmark<StdLib::FloorBenchmark>();
	AddBenchmark<StdLib::RegexBenchmark>();
	AddBenchmark<Web::XmlParseBenchmark>();
	AddBenchmark<Web::JsonParseBenchmark>();
	AddBenchmark<Web::Base64EncodeBenchmark>();
	AddBenchmark<Web::Base64DecodeBenchmark>();
	AddBenchmark<Web::RegexBenchmark>();
}
