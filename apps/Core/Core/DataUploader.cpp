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
// Created by klapeto on 20/6/20.
//

#include "DataUploader.hpp"

#include "Commands/UploadResultCommand.hpp"
#include "Commands/HttpResponseEvent.hpp"
#include "Abstractions/Mediator.hpp"
#include "Abstractions/ResultFormatter.hpp"


#include "httplib.hpp"
#ifdef ELPIDA_WINDOWS
#include <Windows.h>
#endif

#include <Elpida/Utilities/Logging/Logger.hpp>

namespace Elpida
{

	DataUploader::DataUploader(Mediator& mediator, const ResultFormatter& resultFormatter, Logger& logger)
		: _mediator(mediator), _resultFormatter(resultFormatter), _logger(logger)
	{
	}

	void DataUploader::handle(UploadResultCommand& command)
	{
#ifdef ELPIDA_DEBUG_SERVER
		httplib::Client cli(apiUrl, apiPort);
#else
		httplib::SSLClient cli(apiUrl, apiPort);
#endif
		auto res =
			cli.Post(resultPath,
				{{ apiKeyHeader, apiKey }},
				_resultFormatter.serialize(command.getBenchmarkResult()),
				contentType);
		if (res)
		{
			_mediator.execute(HttpResponseEvent(res->body, res->status, std::unordered_map<std::string, std::string>(res->headers.begin(), res->headers.end())));
		}
		else
		{
			_logger.log(LogType::Error, "Error occurred when handling the http request to Elpida Result server");
		}
	}
}