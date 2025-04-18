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


#ifndef ELPIDA_BENCHMARKBASESCORES_HPP
#define ELPIDA_BENCHMARKBASESCORES_HPP

#include <unordered_map>
#include <string>
#include <memory>

#include "FullBenchmarkInstances/AESDecryption.hpp"
#include "FullBenchmarkInstances/AESEncryption.hpp"
#include "FullBenchmarkInstances/Base64Decode.hpp"
#include "FullBenchmarkInstances/Base64Encode.hpp"
#include "FullBenchmarkInstances/CppCompilation.hpp"
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

	class BenchmarkData
	{
	public:
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

		struct GeneratorData
		{
			std::string singleThreadUuid;
			Score singleThreadBaseScore;
			std::string multiThreadUuid;
			Score multiThreadBaseScore;
			GeneratorSignature* generator;
			bool loaded;
		};

		static std::unordered_map<std::string, GeneratorData> GetData()
		{
			return
					{
							{ "AES Decryption",
									{
											"9a44b227-39b1-41d6-b8fc-18243d9c9744",
											94023451.904030,
											"eb8aa408-5d7f-4d8e-8a37-e57edc011c63",
											188096105.887758,
											Generate<AESDecryption>,
											false
									}
							},
							{ "AES Encryption",
									{
											"59248a7c-5d7f-4f1e-b01f-3c3cbe2e7437",
											88540706.813485,
											"9e2fb41c-146f-4725-8ca0-1e19503b03c7",
											177118328.086136,
											Generate<AESEncryption>,
											false
									}
							},
							{ "Base64 Decode",
									{
											"0a48078b-b924-43a9-97d9-6a9fcb2ebb45",
											23164954.909600,
											"060af447-f649-4a5b-b60a-0579616cada0",
											46157315.728114,
											Generate<Base64Decode>,
											false
									}
							},
							{ "Base64 Encode",
									{
											"b793d108-7603-4aa7-b2ae-4c2f8eba98a7",
											182949688.897787,
											"00a8b5a5-eb3e-45b5-bced-f90895103f7d",
											361035939.693451,
											Generate<Base64Encode>,
											false
									}
							},
							{ "C++ Compilation",
									{
											"a60442f2-1bc7-4df6-9d83-18dcda067b51",
											0.418476,
											"b7c330be-90ba-4ac5-a84f-03cd8c77fd85",
											0.779197,
											Generate<CppCompilation>,
											false
									}
							},
							{ "FFT calculation in place",
									{
											"a8946ba5-049f-4626-9b1a-d5d31211cea1",
											16021808.550478,
											"554f2321-95df-43c0-af46-90554e26c8cd",
											32341089.656482,
											Generate<FFT>,
											false
									}
							},
							{ "Json Parsing",
									{
											"cc53b078-75aa-41c0-8885-b381bc1a4bda",
											21214728.551833,
											"35f22cbc-16e7-4c35-aebf-029dac328e46",
											42551977.277542,
											Generate<JsonParsing>,
											false
									}
							},
							{ "Matrix inverse (4x4)",
									{
											"7930adf1-fd99-420a-9a39-b4e147f1bdf2",
											55910428.399221,
											"3faca5e9-d9ac-47a5-a8d4-c331666c6e6a",
											112335203.244437,
											Generate<MatrixInverse>,
											false
									}
							},
							{ "Matrix multiplication (32x32)",
									{
											"56834364-d449-4d3e-bc50-197c697c6c6f",
											64506827.641868,
											"a28c475e-4541-4dcb-80d4-14583c325a7c",
											117211059.983289,
											Generate<MatrixMultiplication>,
											false
									}
							},
							{ "NBody Simulation",
									{
											"9196f126-391f-452b-9ca0-0dd39d7f70ae",
											26758740.309883,
											"6b01f47a-4f41-4f13-96e9-836baf6bed90",
											52022294.310589,
											Generate<NBody>,
											false
									}
							},
							{ "3D Ray Tracing",
									{
											"ad8da8ff-a1f3-4a21-9234-43b6aff8eea4",
											1046626.563159,
											"5c888135-bab1-4651-9407-2d7187f54381",
											2096432.224762,
											Generate<RayTracing>,
											false
									}
							},
							{ "Regex (Boost)",
									{
											"bd0d8849-b988-40ac-9961-5e3528327da3",
											28362263.642960,
											"142291f8-3c83-4f1b-ab02-ad1914e08885",
											57014006.980999,
											Generate<Regex>,
											false
									}
							},
							{ "RSA Decryption",
									{
											"4a4e3a1d-df8b-48d8-8a86-b49fafea79fc",
											31888.275380,
											"bae420c9-3297-456c-bbc2-6b884af15bb7",
											63711.534771,
											Generate<RSADecryption>,
											false
									}
							},
							{ "RSA Encryption",
									{
											"32e5628d-aa9a-4ddc-84d0-17779647e22b",
											1075742.529121,
											"021876da-05b4-4408-8d9a-7637035927c2",
											2151452.615752,
											Generate<RSAEncryption>,
											false
									}
							},
							{ "Svg Rasterization",
									{
											"e7ab603e-6c05-428b-9851-1e0a3729e022",
											268246.380285,
											"66087de6-f486-4d24-b58d-f90300458179",
											536962.850621,
											Generate<SVGRasterization>,
											false
									}
							},
							{ "XML Parsing",
									{
											"70b7048f-face-4976-b68a-d8cd1004ad33",
											32736945.577576,
											"2ac01731-f3de-4fb1-adfc-35937d009097",
											64734615.032234,
											Generate<XmlParsing>,
											false
									}
							},
							{ "Zlib compression",
									{
											"895ff7fd-257d-411f-a8a6-de547b0f6485",
											21301749.490186,
											"1b4532fd-14f0-41af-ab33-5e4e3803f79b",
											42601322.381051,
											Generate<ZlibCompression>,
											false
									}
							},
							{ "Zlib decompression",
									{
											"d40517b1-b39f-4b82-8dc4-b440dd11da1b",
											44505434.986060,
											"39d94225-34aa-479b-b9cb-b6e104fb17b9",
											88978526.279345,
											Generate<ZlibDecompression>,
											false
									}
							}, };

		}
	};
}

#endif //ELPIDA_BENCHMARKBASESCORES_HPP