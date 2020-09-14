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
// Created by klapeto on 25/6/20.
//

#ifndef APPS_QT_CONTROLLERS_UPLOADCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_UPLOADCONTROLLER_HPP

#include "Core/Abstractions/CommandHandler.hpp"

namespace Elpida
{
	class Mediator;
	class BenchmarkResultsModel;
	class Logger;
	class BenchmarkResult;
	class GlobalConfigurationModel;

	class UploadController final : public CommandHandler
	{
	public:
		void handle(HttpResponseEvent& command) override;

		UploadController(Mediator& mediator,
			const BenchmarkResultsModel& benchmarkResultsModel,
			const GlobalConfigurationModel& globalConfigurationModel,
			Logger& logger);
		~UploadController() override = default;
	private:
		Mediator& _mediator;
		const BenchmarkResultsModel& _benchmarkResultsModel;
		const GlobalConfigurationModel& _globalConfigurationModel;
		Logger& _logger;

		void onResultAdded(const BenchmarkResult& result);

		static inline const char* frontEndResultUrl = "https://beta.elpida.dev/result/";
	};
}

#endif //APPS_QT_CONTROLLERS_UPLOADCONTROLLER_HPP
