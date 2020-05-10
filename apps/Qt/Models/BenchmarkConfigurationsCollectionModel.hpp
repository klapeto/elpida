//
// Created by klapeto on 21/4/20.
//

#ifndef APPS_QT_MODELS_BENCHMARKCONFIGURATIONSCOLLECTIONMODEL_HPP
#define APPS_QT_MODELS_BENCHMARKCONFIGURATIONSCOLLECTIONMODEL_HPP

#include <Elpida/Engine/Configuration/Concrete/BenchmarkConfiguration.hpp>
#include "Models/Abstractions/AssociativeModel/AssociativeModel.hpp"

namespace Elpida
{
	class BenchmarkConfigurationsCollectionModel : public AssociativeModel<std::string, BenchmarkConfiguration>
	{

	};
}

#endif //APPS_QT_MODELS_BENCHMARKCONFIGURATIONSCOLLECTIONMODEL_HPP
