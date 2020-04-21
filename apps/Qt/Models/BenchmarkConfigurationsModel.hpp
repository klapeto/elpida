//
// Created by klapeto on 21/4/20.
//

#ifndef APPS_QT_MODELS_BENCHMARKCONFIGURATIONSMODEL_HPP
#define APPS_QT_MODELS_BENCHMARKCONFIGURATIONSMODEL_HPP

#include <Elpida/Engine/Configuration/BenchmarkConfiguration.hpp>
#include "Models/Abstractions/AssociativeModel/AssociativeModel.hpp"

namespace Elpida
{
	class BenchmarkConfigurationsModel : public AssociativeModel<std::string, BenchmarkConfiguration>
	{

	};
}

#endif //APPS_QT_MODELS_BENCHMARKCONFIGURATIONSMODEL_HPP
