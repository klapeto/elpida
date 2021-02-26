/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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
// Created by klapeto on 14/4/20.
//

#ifndef APPS_QT_CONTROLLERS_BENCHMARKSCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_BENCHMARKSCONTROLLER_HPP


#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Models/Benchmarks/BenchmarkGroup.hpp"
#include "Models/Abstractions/AssociativeModel/AssociativeModel.hpp"
#include <Elpida/SharedLibraryLoader.hpp>

namespace Elpida
{
	class Logger;
	class BenchmarkConfiguration;
	class GlobalConfigurationModel;

	template<typename T>
	class BenchmarksContainerPlugin;

	class BenchmarksController
	{
	public:

		void reload();
		void destroyAll();

		explicit BenchmarksController(ListModel<BenchmarkGroup>& model,
			AssociativeModel<std::string, BenchmarkConfiguration>& configurationsModel,
			const GlobalConfigurationModel& globalConfigurationModel,
			const ServiceProvider& serviceProvider,
			Logger& logger);
		virtual ~BenchmarksController();
	private:
		std::vector<std::tuple<std::reference_wrapper<const SharedLibrary>, BenchmarksContainerPlugin<Benchmark>*>> _createdPlugins;
		SharedLibraryLoader _libraryLoader;
		Logger& _logger;
		ListModel<BenchmarkGroup>& _model;
		AssociativeModel<std::string, BenchmarkConfiguration>& _configurationsModel;
		const GlobalConfigurationModel& _globalConfigurationModel;
		const ServiceProvider& _serviceProvider;

		void reloadLibraries();
	};
}

#endif //APPS_QT_CONTROLLERS_BENCHMARKSCONTROLLER_HPP
