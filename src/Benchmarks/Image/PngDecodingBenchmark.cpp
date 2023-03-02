//
// Created by klapeto on 2/3/2023.
//

#include "PngDecodingBenchmark.hpp"

#include "Benchmarks/Common/FileReadTask.hpp"
#include "PngDecodingTask.hpp"

namespace Elpida
{
	BenchmarkInfo PngDecodingBenchmark::GetInfo() const
	{
		FileReadTask read("");
		PngDecodingTask decode;
		return BenchmarkInfo(
			"Png decoding",
			"Decodes a stream of data from PNG decoded data to RGBA data.",
			"B",
			"The input amount of bytes processed per second.",
			{
				read.GetInfo(),
				decode.GetInfo()
			});
	}

	std::vector<std::unique_ptr<Task>> PngDecodingBenchmark::GetTasks() const
	{
		return std::vector<std::unique_ptr<Task>>();
	}

	double PngDecodingBenchmark::CalculateScore(const std::vector<TaskResult>& taskResults) const
	{
		return 0;
	}
} // Elpida