//
// Created by klapeto on 3/9/2023.
//

#ifndef ELPIDA_APPS_APPLICATION_MODELS_CUSTOMBENCHMARKRESULTSMODEL_HPP
#define ELPIDA_APPS_APPLICATION_MODELS_CUSTOMBENCHMARKRESULTSMODEL_HPP

#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"

namespace Elpida::Application
{

	class CustomBenchmarkResultsModel final : public ListModel<BenchmarkResultModel>
	{
	public:
		CustomBenchmarkResultsModel() = default;
	};

} // Application

#endif //ELPIDA_APPS_APPLICATION_MODELS_CUSTOMBENCHMARKRESULTSMODEL_HPP
