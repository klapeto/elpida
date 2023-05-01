/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021 Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 9/5/21.
//

#ifndef ELPIDA_RENTEDCONFIGURATIONVIEW_HPP
#define ELPIDA_RENTEDCONFIGURATIONVIEW_HPP

#include <memory>
#include <vector>
#include "Views/ConfigurationViews/TaskConfigurationListItemViewBase.hpp"
#include "Views/ConfigurationViews/ConfigurationValueViewBase.hpp"

class QVBoxLayout;
class QListWidget;

namespace Elpida
{
	class ConfigurationViewsPool;
	class BenchmarkConfiguration;

	class RentedConfigurationView final
	{
	public:
		RentedConfigurationView(
				const BenchmarkConfiguration& benchmarkConfiguration,
				QListWidget& tasksList,
				QVBoxLayout& configurationContainer,
				ConfigurationViewsPool& configurationViewsPool
		);

		RentedConfigurationView(RentedConfigurationView&&) = default;
		~RentedConfigurationView();
	private:
		std::vector<std::unique_ptr<TaskConfigurationListItemViewBase>> _taskItems;
		std::vector<std::unique_ptr<ConfigurationValueViewBase>> _configurationViews;
		QListWidget& _tasksList;
		QVBoxLayout& _configurationContainer;
		ConfigurationViewsPool& _configurationViewsPool;
	};
}
#endif //ELPIDA_RENTEDCONFIGURATIONVIEW_HPP
