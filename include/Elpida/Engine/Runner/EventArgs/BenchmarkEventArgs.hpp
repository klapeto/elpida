//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RUNNER_EVENTARGS_BENCHMARKEVENTARGS_HPP
#define INCLUDE_ELPIDA_ENGINE_RUNNER_EVENTARGS_BENCHMARKEVENTARGS_HPP


namespace Elpida
{
	class Benchmark;

	class BenchmarkEventArgs
	{
	public:

		[[nodiscard]] const Benchmark& getBenchmark() const
		{
			return _benchmark;
		}

		explicit BenchmarkEventArgs(const Benchmark& benchmark)
			: _benchmark(benchmark)
		{

		}
		virtual ~BenchmarkEventArgs() = default;
	private:
		const Benchmark& _benchmark;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_RUNNER_EVENTARGS_BENCHMARKEVENTARGS_HPP
