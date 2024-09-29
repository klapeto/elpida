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

#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include "FullBenchmarkInstances/AESDecryption.hpp"
#include "FullBenchmarkInstances/AESEncryption.hpp"
#include "FullBenchmarkInstances/Base64Decode.hpp"
#include "FullBenchmarkInstances/Base64Encode.hpp"
#include "FullBenchmarkInstances/FFT.hpp"
#include "FullBenchmarkInstances/MatrixInverse.hpp"
#include "FullBenchmarkInstances/MatrixMultiplication.hpp"
#include "FullBenchmarkInstances/NBody.hpp"
#include "FullBenchmarkInstances/JsonParsing.hpp"
#include "FullBenchmarkInstances/RayTracing.hpp"
#include "FullBenchmarkInstances/Regex.hpp"
#include "FullBenchmarkInstances/RSADecryption.hpp"
#include "FullBenchmarkInstances/RSAEncryption.hpp"
#include "FullBenchmarkInstances/SvgRasterization.hpp"
#include "FullBenchmarkInstances/XmlParsing.hpp"
#include "FullBenchmarkInstances/ZlibCompression.hpp"
#include "FullBenchmarkInstances/ZlibDecompression.hpp"

namespace Elpida::Application
{
	using GeneratorSignature = std::unique_ptr<FullBenchmarkInstance>(
			const std::string& singleThreadUuid,
			Score singleThreadBaseScore,
			const std::string& multiThreadUuid,
			Score multiThreadBaseScore,
			const BenchmarkModel& benchmarkModel,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			const BenchmarkRunConfigurationModel& runConfigurationModel,
			BenchmarkExecutionService& benchmarkExecutionService,
			bool multiThread);

	struct GeneratorData
	{
		std::string singleThreadUuid;
		Score singleThreadBaseScore;
		std::string multiThreadUuid;
		Score multiThreadBaseScore;
		GeneratorSignature* generator;
	};

	template<typename T>
	static std::unique_ptr<FullBenchmarkInstance> Generate(
			const std::string& singleThreadUuid,
			Score singleThreadBaseScore,
			const std::string& multiThreadUuid,
			Score multiThreadBaseScore,
			const BenchmarkModel& benchmarkModel,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			const BenchmarkRunConfigurationModel& runConfigurationModel,
			BenchmarkExecutionService& benchmarkExecutionService,
			bool multiThread)
	{
		if (multiThread)
		{
			return std::make_unique<T>(multiThreadUuid,
					multiThreadBaseScore,
					true,
					benchmarkModel,
					timingModel,
					topologyModel,
					memoryInfoModel,
					runConfigurationModel,
					benchmarkExecutionService);
		}
		else
		{
			return std::make_unique<T>(singleThreadUuid,
					singleThreadBaseScore,
					false,
					benchmarkModel,
					timingModel,
					topologyModel,
					memoryInfoModel,
					runConfigurationModel,
					benchmarkExecutionService);
		}
	}

	std::vector<std::unique_ptr<FullBenchmarkInstance>> FullBenchmarkInstancesLoader::Load(
			const std::vector<BenchmarkGroupModel>& benchmarkGroups,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			const BenchmarkRunConfigurationModel& runConfigurationModel,
			BenchmarkExecutionService& benchmarkExecutionService)
	{

		std::unordered_map<std::string, GeneratorData> generators =
				{
						{ "AES Decryption", {
								"9a44b227-39b1-41d6-b8fc-18243d9c9744",
								205.50 * std::mega::num,
								"eb8aa408-5d7f-4d8e-8a37-e57edc011c63",
								408.41 * std::mega::num,
								Generate<AESDecryption>
						}
						},
						{ "AES Encryption", {
								"59248a7c-5d7f-4f1e-b01f-3c3cbe2e7437",
								158.88 * std::mega::num,
								"9e2fb41c-146f-4725-8ca0-1e19503b03c7",
								316.86 * std::mega::num,
								Generate<AESEncryption>
						}
						},
						{ "Base64 Decode", {
								"0a48078b-b924-43a9-97d9-6a9fcb2ebb45",
								38.81 * std::mega::num,
								"060af447-f649-4a5b-b60a-0579616cada0",
								77.29 * std::mega::num,
								Generate<Base64Decode>
						}
						},
						{ "Base64 Encode", {
								"b793d108-7603-4aa7-b2ae-4c2f8eba98a7",
								149.16 * std::mega::num,
								"00a8b5a5-eb3e-45b5-bced-f90895103f7d",
								339.79 * std::mega::num,
								Generate<Base64Encode>
						}
						},
						{ "FFT calculation in place", {
								"a8946ba5-049f-4626-9b1a-d5d31211cea1",
								10.70 * std::mega::num,
								"554f2321-95df-43c0-af46-90554e26c8cd",
								21.29 * std::mega::num,
								Generate<FFT>
						}
						},
						{ "Json Parsing", {
								"cc53b078-75aa-41c0-8885-b381bc1a4bda",
								20.24 * std::mega::num,
								"35f22cbc-16e7-4c35-aebf-029dac328e46",
								40.76 * std::mega::num,
								Generate<JsonParsing>
						}
						},
						{ "Matrix inverse (4x4)", {
								"7930adf1-fd99-420a-9a39-b4e147f1bdf2",
								52.71 * std::mega::num,
								"3faca5e9-d9ac-47a5-a8d4-c331666c6e6a",
								105.30 * std::mega::num,
								Generate<MatrixInverse>
						}
						},
						{ "Matrix multiplication (32x32)", {
								"56834364-d449-4d3e-bc50-197c697c6c6f",
								73.33 * std::mega::num,
								"a28c475e-4541-4dcb-80d4-14583c325a7c",
								145.58 * std::mega::num,
								Generate<MatrixMultiplication>
						}
						},
						{ "NBody Simulation", {
								"9196f126-391f-452b-9ca0-0dd39d7f70ae",
								19.98 * std::mega::num,
								"6b01f47a-4f41-4f13-96e9-836baf6bed90",
								39.92 * std::mega::num,
								Generate<NBody>
						}
						},
						{ "RSA Decryption", {
								"4a4e3a1d-df8b-48d8-8a86-b49fafea79fc",
								96.84 * std::kilo::num,
								"bae420c9-3297-456c-bbc2-6b884af15bb7",
								193.68 * std::kilo::num,
								Generate<RSADecryption>
						}
						},
						{ "RSA Encryption", {
								"32e5628d-aa9a-4ddc-84d0-17779647e22b",
								2.06 * std::mega::num,
								"021876da-05b4-4408-8d9a-7637035927c2",
								4.10 * std::mega::num,
								Generate<RSAEncryption>
						}
						},
						{ "3D Ray Tracing", {
								"ad8da8ff-a1f3-4a21-9234-43b6aff8eea4",
								940.20 * std::kilo::num,
								"5c888135-bab1-4651-9407-2d7187f54381",
								1.87 * std::mega::num,
								Generate<RayTracing>
						}
						},
						{ "Regex (Boost)", {
								"bd0d8849-b988-40ac-9961-5e3528327da3",
								19.33 * std::mega::num,
								"142291f8-3c83-4f1b-ab02-ad1914e08885",
								37.79 * std::mega::num,
								Generate<Regex>
						}
						},
						{ "Svg Rasterization", {
								"e7ab603e-6c05-428b-9851-1e0a3729e022",
								268.78 * std::kilo::num,
								"66087de6-f486-4d24-b58d-f90300458179",
								533.83 * std::kilo::num,
								Generate<SvgRasterization>
						}
						},
						{ "XML Parsing", {
								"70b7048f-face-4976-b68a-d8cd1004ad33",
								39.37 * std::mega::num,
								"2ac01731-f3de-4fb1-adfc-35937d009097",
								70.12 * std::mega::num,
								Generate<XmlParsing>
						}
						},
						{ "Zlib compression", {
								"895ff7fd-257d-411f-a8a6-de547b0f6485",
								21.38 * std::mega::num,
								"1b4532fd-14f0-41af-ab33-5e4e3803f79b",
								42.53 * std::mega::num,
								Generate<ZlibCompression>
						}
						},
						{ "Zlib decompression", {
								"d40517b1-b39f-4b82-8dc4-b440dd11da1b",
								45.30 * std::mega::num,
								"39d94225-34aa-479b-b9cb-b6e104fb17b9",
								90.34 * std::mega::num,
								Generate<ZlibDecompression>
						}
						}
				};

		// wierd code to sort it correctly : Aphabetically and single/multi
		std::vector<std::tuple<std::unique_ptr<FullBenchmarkInstance>, GeneratorData, const BenchmarkModel*>> firstBatch;
		for (auto& group : benchmarkGroups)
		{
			for (auto& benchmark : group.GetBenchmarks())
			{
				auto& name = benchmark.GetName();

				auto itr = generators.find(name);
				if (itr != generators.end())
				{
					auto& data = itr->second;
					firstBatch.emplace_back(data.generator(
							data.singleThreadUuid,
							data.singleThreadBaseScore,
							data.multiThreadUuid,
							data.multiThreadBaseScore,
							benchmark,
							timingModel,
							topologyModel,
							memoryInfoModel,
							runConfigurationModel,
							benchmarkExecutionService, false), itr->second, &benchmark);
				}
			}
		}

		std::sort(firstBatch.begin(), firstBatch.end(),
				[](auto& a, auto& b)
				{
					return std::get<0>(a)->GetName() < std::get<0>(b)->GetName();
				});

		std::vector<std::unique_ptr<FullBenchmarkInstance>> benchmarks;

		std::sort(benchmarks.begin(), benchmarks.end(),
				[](std::unique_ptr<FullBenchmarkInstance>& a, std::unique_ptr<FullBenchmarkInstance>& b)
				{
					return a->GetName() < b->GetName();
				});

		for (auto& pair: firstBatch)
		{
			benchmarks.push_back(std::move(std::get<0>(pair)));
		}

		for (auto& tuple: firstBatch)
		{
			auto& data = std::get<1>(tuple);
			auto& benchmark = *std::get<2>(tuple);
			benchmarks.push_back(data.generator(
					data.singleThreadUuid,
					data.singleThreadBaseScore,
					data.multiThreadUuid,
					data.multiThreadBaseScore,
					benchmark,
					timingModel,
					topologyModel,
					memoryInfoModel,
					runConfigurationModel,
					benchmarkExecutionService, true));
		}

		return benchmarks;
	}
} // Application
// Elpida