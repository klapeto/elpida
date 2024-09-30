//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 12/3/2023.
//

#ifndef ELPIDA_ARGUMENTSVALIDATOR_HPP_
#define ELPIDA_ARGUMENTSVALIDATOR_HPP_

#include <string>

namespace Elpida::Application
{
	enum class OutputFormat
	{
		Default,
		Json
	};

	class ArgumentsHelper final
	{
	public:
		[[nodiscard]]
		const std::string& GetBenchmarksPath() const;

		[[nodiscard]]
		const std::string& GetReportPath() const;

		[[nodiscard]]
		size_t GetIterationsToRun() const;

		[[nodiscard]]
		std::size_t GetSecondsBetweenRuns() const;

		[[nodiscard]]
		OutputFormat GetOutputFormat() const;

		[[nodiscard]]
		bool IsGenerateReport() const;

		[[nodiscard]]
		bool IsUpload() const;

		[[nodiscard]]
		const std::string& GetResultPath() const;

		[[nodiscard]]
		bool IsQuiet() const
		{
			return _quiet;
		}

		[[nodiscard]]
		bool ParseAndGetExitText(int argC, char* argV[], std::string& returnText);

		ArgumentsHelper();
		~ArgumentsHelper() = default;

	private:
		std::string _benchmarksPath;
		std::string _reportPath;
		std::string _resultPath;
		std::size_t _iterationsToRun;
		std::size_t _secondsBetweenRuns;
		OutputFormat _outputFormat;
		bool _generateReport;
		bool _upload;
		bool _quiet;
		static std::string GetHelpString();
		static std::string GetVersionString();
		void ParseFormat(const std::string& value);
	};

} // Elpida::Application

#endif //ELPIDA_ARGUMENTSVALIDATOR_HPP_
