//
// Created by klapeto on 12/4/20.
//

#ifndef APPS_QT_CORE_MODELS_TASKBATCHESMODEL_HPP
#define APPS_QT_CORE_MODELS_TASKBATCHESMODEL_HPP

#include "Models/Abstractions/ListModel/ListModel.hpp"

namespace Elpida
{
	class Benchmark;

	class BenchmarksModel : public ListModel<Benchmark*>
	{

	};
}


#endif //APPS_QT_CORE_MODELS_TASKBATCHESMODEL_HPP
