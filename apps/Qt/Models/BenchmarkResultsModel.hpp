//
// Created by klapeto on 17/4/20.
//

#ifndef APPS_QT_MODELS_BENCHMARKRESULTSMODEL_HPP
#define APPS_QT_MODELS_BENCHMARKRESULTSMODEL_HPP

#include <Elpida/Engine/Result/BenchmarkResult.hpp>
#include "Models/Abstractions/ListModel/ListModel.hpp"

namespace Elpida
{
	class BenchmarkResultsModel : public ListModel<BenchmarkResult>
	{

	};
}


#endif //APPS_QT_MODELS_BENCHMARKRESULTSMODEL_HPP
