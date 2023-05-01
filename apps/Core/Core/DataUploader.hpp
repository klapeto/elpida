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

#ifndef APPS_QT_CORE_DATAUPLOADER_HPP
#define APPS_QT_CORE_DATAUPLOADER_HPP

#include <Elpida/Engine/Result/BenchmarkResult.hpp>
#include "Abstractions/CommandHandler.hpp"

#ifndef ELPIDA_WEB_API_KEY
#define ELPIDA_WEB_API_KEY "Test API Key"
#endif

#ifndef ELPIDA_WEB_API_URL
#define ELPIDA_WEB_API_URL "localhost"
#endif

namespace Elpida
{
	class Mediator;
	class ResultFormatter;
	class Logger;

	class DataUploader final : public CommandHandler
	{
	public:
		void handle(UploadResultCommand& command) override;

		explicit DataUploader(Mediator& mediator, const ResultFormatter& resultFormatter, Logger& logger);
		~DataUploader() override = default;
	private:
		Mediator& _mediator;
		const ResultFormatter& _resultFormatter;
		Logger& _logger;

#ifdef ELPIDA_DEBUG_SERVER
		const int apiPort = 5000;
		static inline const char* apiUrl = "localhost";
#else
		const int apiPort = 443;
		static inline const char* apiUrl = ELPIDA_WEB_API_URL;
#endif
		static inline const char* resultPath = "/api/v1/benchmarkresult";
		static inline const char* contentType = "application/json";
		static inline const char* apiKey = ELPIDA_WEB_API_KEY;
		static inline const char* apiKeyHeader = "api_key";
	};
}

#endif //APPS_QT_CORE_DATAUPLOADER_HPP
