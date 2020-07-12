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

		static inline const char* apiUrl = "https://beta.elpida.dev/api/result";
	};
}

#endif //APPS_QT_CORE_DATAUPLOADER_HPP
