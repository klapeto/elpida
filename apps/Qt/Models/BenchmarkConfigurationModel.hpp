//
// Created by klapeto on 23/4/20.
//

#ifndef APPS_QT_MODELS_BENCHMARKCONFIGURATIONMODEL_HPP
#define APPS_QT_MODELS_BENCHMARKCONFIGURATIONMODEL_HPP

#include "Models/Abstractions/Model.hpp"

namespace Elpida
{
	class Benchmark;
	class BenchmarkConfiguration;

	class BenchmarkConfigurationModel : public Model
	{
	public:

		Benchmark* getBenchmark() const
		{
			return _benchmark;
		}
		BenchmarkConfiguration* getBenchmarkConfiguration() const
		{
			return _benchmarkConfiguration;
		}

		void setBenchmark(Benchmark* benchmark)
		{
			_benchmark = benchmark;
			onDataChanged();
		}

		void setBenchmarkConfiguration(BenchmarkConfiguration* benchmarkConfiguration)
		{
			_benchmarkConfiguration = benchmarkConfiguration;
			onDataChanged();
		}

		BenchmarkConfigurationModel()
			: _benchmark(nullptr), _benchmarkConfiguration(nullptr)
		{

		}
		~BenchmarkConfigurationModel() override = default;
	private:
		Benchmark* _benchmark;
		BenchmarkConfiguration* _benchmarkConfiguration;
	};
}


#endif //APPS_QT_MODELS_BENCHMARKCONFIGURATIONMODEL_HPP
