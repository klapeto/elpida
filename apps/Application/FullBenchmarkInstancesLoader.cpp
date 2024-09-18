//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
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
// Created by klapeto on 17/9/2024.
//

#include "FullBenchmarkInstancesLoader.hpp"

#include "FullBenchmarkInstances/SvgRasterizationSingleThread.hpp"
#include "FullBenchmarkInstances/SvgRasterizationMultiThread.hpp"
#include "FullBenchmarkInstances/ParseJsonSingleThread.hpp"
#include "FullBenchmarkInstances/ParseJsonMultiThread.hpp"
#include "FullBenchmarkInstances/ZlibCompressionSingleThread.hpp"
#include "FullBenchmarkInstances/ZlibCompressionMultiThread.hpp"
#include "FullBenchmarkInstances/ZlibDecompressionSingleThread.hpp"
#include "FullBenchmarkInstances/ZlibDecompressionMultiThread.hpp"
#include "FullBenchmarkInstances/RSADecryptionMultiThread.hpp"
#include "FullBenchmarkInstances/RSADecryptionSingleThread.hpp"
#include "FullBenchmarkInstances/RSAEncryptionMultiThread.hpp"
#include "FullBenchmarkInstances/RSAEncryptionSingleThread.hpp"
#include "FullBenchmarkInstances/AESDecryptionMultiThread.hpp"
#include "FullBenchmarkInstances/AESDecryptionSingleThread.hpp"
#include "FullBenchmarkInstances/AESEncryptionMultiThread.hpp"
#include "FullBenchmarkInstances/AESEncryptionSingleThread.hpp"
#include "FullBenchmarkInstances/Base64EncodeMultiThread.hpp"
#include "FullBenchmarkInstances/Base64EncodeSingleThread.hpp"
#include "FullBenchmarkInstances/Base64DecodeMultiThread.hpp"
#include "FullBenchmarkInstances/Base64DecodeSingleThread.hpp"
#include "FullBenchmarkInstances/FFTMultiThread.hpp"
#include "FullBenchmarkInstances/FFTSingleThread.hpp"
#include "FullBenchmarkInstances/MatrixMultiplicationMultiThread.hpp"
#include "FullBenchmarkInstances/MatrixMultiplicationSingleThread.hpp"
#include "FullBenchmarkInstances/MatrixInverseMultiThread.hpp"
#include "FullBenchmarkInstances/MatrixInverseSingleThread.hpp"
#include "FullBenchmarkInstances/NBodyMultiThread.hpp"
#include "FullBenchmarkInstances/NBodySingleThread.hpp"
#include "FullBenchmarkInstances/RayTracingMultiThread.hpp"
#include "FullBenchmarkInstances/RayTracingSingleThread.hpp"
#include "FullBenchmarkInstances/RegexMultiThread.hpp"
#include "FullBenchmarkInstances/RegexSingleThread.hpp"
#include "FullBenchmarkInstances/XmlParsingMultiThread.hpp"
#include "FullBenchmarkInstances/XmlParsingSingleThread.hpp"


namespace Elpida::Application
{
	std::vector<std::unique_ptr<FullBenchmarkInstance>> FullBenchmarkInstancesLoader::Load(
			const std::vector<BenchmarkGroupModel>& benchmarkGroups,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			BenchmarkExecutionService& benchmarkExecutionService)
	{
		std::vector<std::unique_ptr<FullBenchmarkInstance>> _benchmarks;
		for (auto& group : benchmarkGroups)
		{
			for (auto& benchmark : group.GetBenchmarks())
			{
				auto& name = benchmark.GetName();
				if (name == "Svg Rasterization")
				{
					_benchmarks.push_back(
							std::make_unique<SvgRasterizationSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<SvgRasterizationMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Json Parsing")
				{
					_benchmarks.push_back(
							std::make_unique<ParseJsonSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<ParseJsonMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Zlib compression")
				{
					_benchmarks.push_back(
							std::make_unique<ZlibCompressionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<ZlibCompressionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Zlib decompression")
				{
					_benchmarks.push_back(
							std::make_unique<ZlibDecompressionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<ZlibDecompressionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "RSA Encryption")
				{
					_benchmarks.push_back(
							std::make_unique<RSAEncryptionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<RSAEncryptionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "RSA Decryption")
				{
					_benchmarks.push_back(
							std::make_unique<RSADecryptionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<RSADecryptionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "AES Encryption")
				{
					_benchmarks.push_back(
							std::make_unique<AESEncryptionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<AESEncryptionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "AES Decryption")
				{
					_benchmarks.push_back(
							std::make_unique<AESDecryptionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<AESDecryptionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Base64 Encode")
				{
					_benchmarks.push_back(
							std::make_unique<Base64EncodeSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<Base64EncodeMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Base64 Decode")
				{
					_benchmarks.push_back(
							std::make_unique<Base64DecodeSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<Base64DecodeMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "FFT calculation in place")
				{
					_benchmarks.push_back(
							std::make_unique<FFTSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<FFTMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Matrix multiplication (32x32)")
				{
					_benchmarks.push_back(
							std::make_unique<MatrixMultiplicationSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<MatrixMultiplicationMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Matrix inverse (4x4)")
				{
					_benchmarks.push_back(
							std::make_unique<MatrixInverseSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<MatrixInverseMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "NBody Simulation")
				{
					_benchmarks.push_back(
							std::make_unique<NBodySingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<NBodyMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "3D Ray Tracing")
				{
					_benchmarks.push_back(
							std::make_unique<RayTracingSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<RayTracingMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Regex")
				{
					_benchmarks.push_back(
							std::make_unique<RegexSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<RegexMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "XML Parsing")
				{
					_benchmarks.push_back(
							std::make_unique<XmlParsingSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<XmlParsingMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
			}
		}

		return _benchmarks;
	}
} // Application
// Elpida