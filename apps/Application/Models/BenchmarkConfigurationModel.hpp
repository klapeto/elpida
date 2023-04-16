//
// Created by klapeto on 16/4/2023.
//

#ifndef ELPIDA_BENCHMARKCONFIGURATIONMODEL_HPP_
#define ELPIDA_BENCHMARKCONFIGURATIONMODEL_HPP_

#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Models/BenchmarkConfigurationInstanceModel.hpp"

namespace Elpida::Application
{

	class BenchmarkConfigurationModel : public ListModel<BenchmarkConfigurationInstanceModel>
	{
	 public:
		BenchmarkConfigurationModel() = default;
		~BenchmarkConfigurationModel() override = default;
	};

} // Application

#endif //ELPIDA_BENCHMARKCONFIGURATIONMODEL_HPP_
