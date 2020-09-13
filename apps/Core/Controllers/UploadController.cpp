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

#include "UploadController.hpp"

#include "Models/BenchmarkResultsModel.hpp"
#include "Models/GlobalConfigurationModel.hpp"
#include "Core/Abstractions/Mediator.hpp"
#include "Core/Commands/UploadResultCommand.hpp"
#include "Core/Commands/HttpResponseEvent.hpp"
#include "Utilities/OsUtilities.hpp"
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>

namespace Elpida
{
	UploadController::UploadController(Mediator& mediator,
		const BenchmarkResultsModel& benchmarkResultsModel,
		const GlobalConfigurationModel& globalConfigurationModel,
		Logger& logger)
		: _mediator(mediator), _benchmarkResultsModel(benchmarkResultsModel),
		  _globalConfigurationModel(globalConfigurationModel), _logger(logger)
	{
		_benchmarkResultsModel.itemAdded.subscribe([this](const CollectionChangedEventArgs<BenchmarkResult>& item)
		{
			onResultAdded(item.getItem().getValue());
		});
	}

	void UploadController::handle(HttpResponseEvent& command)
	{
		if (command.getResponseCode() != 201)
		{
			_logger.log(LogType::Warning,
				Vu::Cs("Upload result to elpida server did not succeed. Http Resp. Code: '",
					command.getResponseCode(),
					"'. Message: '",
					command.getResponse(),
					"'"));
			return;
		}
		if (_globalConfigurationModel.isOpenResultsWebPage())
		{
			auto itr = command.getResponseHeaders().find("Location");

			if (itr != command.getResponseHeaders().end())
			{
				auto index = itr->second.find_last_of('/') + 1;	// API returns a URI to api. We need only the id
				if (index < itr->second.size())
				{
					auto id = itr->second.substr(index);
					OsUtilities::openUrl(std::string(frontEndResultUrl) + id);
				}
				else
				{
					_logger.log(LogType::Warning,
						"Elpida server accepted the result but did not provide the a valid result URL on its headers");
				}
			}
			else
			{
				_logger.log(LogType::Warning,
					"Elpida server accepted the result but did not provide the result URL on its headers");
			}
		}
	}

	void UploadController::onResultAdded(const BenchmarkResult& result)
	{
		if (_globalConfigurationModel.isUploadResults())
		{
			_mediator.execute(UploadResultCommand(result));
		}
	}
}