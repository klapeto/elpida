//
// Created by klapeto on 17/4/20.
//

#ifndef APPS_QT_MODELS_TASKRUNRESULTSMODEL_HPP
#define APPS_QT_MODELS_TASKRUNRESULTSMODEL_HPP

#include "Models/Abstractions/CollectionModel.hpp"
#include <Elpida/TaskBatchRunResult.hpp>

namespace Elpida
{
	class TaskRunResultsModel : public CollectionModel<TaskBatchRunResult>
	{


	};
}


#endif //APPS_QT_MODELS_TASKRUNRESULTSMODEL_HPP
