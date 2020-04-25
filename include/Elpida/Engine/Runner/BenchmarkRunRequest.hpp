//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RUNNER_BENCHMARKRUNREQUEST_HPP
#define INCLUDE_ELPIDA_ENGINE_RUNNER_BENCHMARKRUNREQUEST_HPP

namespace Elpida
{
	class Benchmark;
	class BenchmarkConfiguration;

	class BenchmarkRunRequest final
	{
	public:

		const Benchmark& getBenchmark() const
		{
			return _benchmark;
		}
		const BenchmarkConfiguration& getConfiguration() const
		{
			return _configuration;
		}

		BenchmarkRunRequest(const Benchmark& benchmark, const BenchmarkConfiguration& configuration)
			: _benchmark(benchmark), _configuration(configuration)
		{
		}

		~BenchmarkRunRequest() = default;
	private:
		const Benchmark& _benchmark;
		const BenchmarkConfiguration& _configuration;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RUNNER_BENCHMARKRUNREQUEST_HPP
