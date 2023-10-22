//
// Created by klapeto on 22/10/2023.
//

#include "FullBenchmarkModel.hpp"

namespace Elpida::Application
{
	FullBenchmarkModel::FullBenchmarkModel(const std::vector<BenchmarkGroupModel>& benchmarkGroups)
			: _memoryLatency(nullptr), _memoryReadBandwidth(nullptr), _pngEncoding(nullptr)
	{
		for (auto& group: benchmarkGroups)
		{
			for (auto& benchmark: group.GetBenchmarks())
			{
				auto name = benchmark.GetName();
				if (name == "Memory latency")
				{
					_memoryLatency = &benchmark;
				}
				else if (name == "Memory read bandwidth")
				{
					_memoryReadBandwidth = &benchmark;
				}
				else if (name == "Png Encoding/Decoding")
				{
					_pngEncoding = &benchmark;
				}
			}
		}
		if (!_memoryReadBandwidth || !_memoryLatency)
		{
			throw ElpidaException("Missing benchmarks: Memory benchmarks");
		}

		if (!_pngEncoding)
		{
			throw ElpidaException("Missing benchmarks: Image benchmarks");
		}
	}
} // Elpida
// Application