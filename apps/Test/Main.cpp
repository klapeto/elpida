//
// Created by klapeto on 27/2/2023.
//

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <png.h>
#include <fstream>
#include <filesystem>

#include "Benchmarks/Memory/MemoryLatencyBenchmark.hpp"
#include "Benchmarks/Memory/MemoryReadBandwidthBenchmark.hpp"
#include "Benchmarks/Image/PngEncodingDecodingBenchmark.hpp"
#include "Elpida/ValueUtilities.hpp"
#include "Elpida/OverheadsInfo.hpp"

using namespace Elpida;

static void Load()
{
	std::srand(std::time(nullptr));
	auto salt = (std::size_t)(std::rand() / (float)(RAND_MAX) * 10);
	std::size_t x = 1000000000;
	auto f = 0.0f;
	while (x--)
	{
		f += (float)x / (float)(salt + x);
	}
	std::cout << "Done with: " << f << std::endl;
}

std::string TranslateResult(const BenchmarkResult& result, const BenchmarkInfo& benchmarkInfo)
{
	return ValueUtilities::GetValueScaleStringSI(result.GetScore()) + benchmarkInfo.GetScoreUnit();
}

int main(int argC, char* argV[])
{

//	png_image img;
//	img.opaque = nullptr;
//	img.version = PNG_IMAGE_VERSION;
//
//	auto size =std::filesystem::file_size("/home/klapeto/Εικόνες/Elpida-poster-9.png");
//	auto mem = new char[size];
//	std::fstream file("/home/klapeto/Εικόνες/Elpida-poster-9.png", std::ios::in| std::ios::binary);
//
//	file.read(mem, size);
//
//	png_image_begin_read_from_memory(&img, mem, size);
//
//	img.format = PNG_FORMAT_RGBA;
//
//	auto buffer = new char[PNG_IMAGE_SIZE(img)];
//
//	png_image_finish_read(&img, nullptr, buffer, 0, nullptr);


	PngEncodingDecodingBenchmark benchmark;



	EnvironmentInfo environmentInfo((OverheadsInfo()), TopologyInfo());

	std::vector<TaskConfiguration> taskConfiguration = benchmark.GetRequiredConfiguration();

	taskConfiguration[0].SetValue("/home/klapeto/Εικόνες/Elpida-poster-9.png");
	taskConfiguration[1].SetValue("/home/klapeto/Εικόνες/Elpida-poster-9_out.png");
	//Load();

	auto result = benchmark.Run(environmentInfo.GetTopologyInfo().GetAllProcessingUnits(), taskConfiguration, environmentInfo);

	std::cout << "Result: " << TranslateResult(result, benchmark.GetInfo()) << std::endl;

	return EXIT_SUCCESS;
}