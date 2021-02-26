/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 23/4/20.
//

#ifndef APPS_QT_MODELS_BENCHMARKCONFIGURATIONMODEL_HPP
#define APPS_QT_MODELS_BENCHMARKCONFIGURATIONMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include <Elpida/Utilities/OptionalReference.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>

namespace Elpida
{
	class BenchmarkConfiguration;

	class BenchmarkConfigurationModel : public Model
	{
	public:

		OptionalReference<Benchmark> getBenchmark() const
		{
			return _benchmark;
		}

		OptionalReference<BenchmarkConfiguration> getBenchmarkConfiguration() const
		{
			return _benchmarkConfiguration;
		}

		void setBenchmark(OptionalReference<Benchmark> benchmark)
		{
			_benchmark = benchmark;
			onDataChanged();
		}

		void setBenchmarkConfiguration(OptionalReference<BenchmarkConfiguration> benchmarkConfiguration)
		{
			_benchmarkConfiguration = benchmarkConfiguration;
			onDataChanged();
		}

		BenchmarkConfigurationModel()
			: _benchmark(std::nullopt),
			_benchmarkConfiguration(std::nullopt)
		{

		}
		~BenchmarkConfigurationModel() override = default;
	private:
		OptionalReference<Benchmark> _benchmark;
		OptionalReference<BenchmarkConfiguration> _benchmarkConfiguration;
	};
}


#endif //APPS_QT_MODELS_BENCHMARKCONFIGURATIONMODEL_HPP
