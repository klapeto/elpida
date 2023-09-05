//
// Created by klapeto on 3/9/2023.
//

#ifndef ELPIDA_APPS_APPLICATION_MODELS_BENCHMARKRESULTSMODEL_HPP
#define ELPIDA_APPS_APPLICATION_MODELS_BENCHMARKRESULTSMODEL_HPP

#include "Abstractions/ListModel/ListModel.hpp"
#include "BenchmarkResultModel.hpp"

namespace Elpida::Application
{

	class BenchmarkResultsModel final : public ListModel<BenchmarkResultModel>
	{
	public:
		BenchmarkResultsModel() = default;
	};

} // Application

#endif //ELPIDA_APPS_APPLICATION_MODELS_BENCHMARKRESULTSMODEL_HPP
