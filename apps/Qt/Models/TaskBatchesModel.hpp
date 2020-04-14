//
// Created by klapeto on 12/4/20.
//

#ifndef APPS_QT_CORE_MODELS_TASKBATCHESMODEL_HPP
#define APPS_QT_CORE_MODELS_TASKBATCHESMODEL_HPP

#include "Models/Abstractions/CollectionModel.hpp"

namespace Elpida
{
	class QtTaskBatchWrapper;

	class TaskBatchesModel : public CollectionModel<QtTaskBatchWrapper*>
	{

	};
}


#endif //APPS_QT_CORE_MODELS_TASKBATCHESMODEL_HPP
