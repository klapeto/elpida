//
// Created by klapeto on 17/4/20.
//

#ifndef APPS_QT_MODELS_TASKRUNRESULTSMODEL_HPP
#define APPS_QT_MODELS_TASKRUNRESULTSMODEL_HPP

#include <Elpida/Engine/Result/BenchmarkResult.hpp>
#include "Models/Abstractions/CollectionModel.hpp"

namespace Elpida
{
	class TaskRunResultsModel : public CollectionModel<BenchmarkResult>
	{

	};
}


#endif //APPS_QT_MODELS_TASKRUNRESULTSMODEL_HPP
