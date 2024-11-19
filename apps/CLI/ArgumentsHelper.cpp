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

#include "ArgumentsHelper.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#ifdef _MSC_VER
#include "getopt_MSVC.h"
#else

#include <getopt.h>

#endif

namespace Elpida::Application
{

	String ArgumentsHelper::GetHelpString()
	{
		std::ostringstream accumulator;
		accumulator << R"(Elpida CLI: )" << ELPIDA_VERSION << std::endl;
		accumulator << R"(Example usage: elpida-cli --format=json --upload ...)"<< std::endl;
		accumulator << R"(       -v, --version)" << std::endl;
		accumulator << R"(           Prints the version and exits)" << std::endl;
		accumulator << R"(       -h, --help)" << std::endl;
		accumulator << R"(           Prints this help and exits)" << std::endl;
		accumulator << R"(       --format=FORMAT)" << std::endl;
		accumulator << R"(           The result output format. Accepted values (default, json))" << std::endl;
		accumulator << R"(       --delay-seconds=SECONDS)" << std::endl;
		accumulator << R"(           The delay in seconds between runs (Default is 0))" << std::endl;
		accumulator << R"(       --upload)" << std::endl;
		accumulator << R"(           Enable uploading the result to elpida website (disabled by default))" << std::endl;
		accumulator << R"(       --generate-report=[OUTPUT_FILE])" << std::endl;
		accumulator << R"(           Whether to generate an HTML report. If no path is specified it is output to 'Elpida-Report.html')" << std::endl;
		accumulator << R"(       --iterations=N)" << std::endl;
		accumulator << R"(           How many iterations to run (Default is 1))" << std::endl;
		accumulator << R"(       --benchmarks-directory=DIRECTORY)" << std::endl;
		accumulator << R"(           Where to find the benchmarks (Default is './Benchmarks'))" << std::endl;
		accumulator << R"(       --output=FILE)" << std::endl;
		accumulator << R"(           Output the results on a file instead of STDOUT)" << std::endl;
		accumulator << R"(       --quiet)" << std::endl;
		accumulator << R"(           Output only the results on STDOUT)" << std::endl;

		return accumulator.str();
	}

	static Size ParseUnsigned(const String& value, const char* name)
	{
		if (value.empty())
		{
			throw ElpidaException("'--", name, "' option cannot be empty");
		}
		try
		{
			return std::stoul(value);
		}
		catch (const std::invalid_argument& e)
		{
			throw ElpidaException("'--", name, "' has invalid value: ", value);
		}
		catch (const std::out_of_range& e)
		{
			throw ElpidaException("'--", name, "' has value out of range: ", value);
		}
	}

	String ArgumentsHelper::GetVersionString()
	{
		std::ostringstream accumulator;

		accumulator << "Elpida Benchmark Executor: " << ELPIDA_VERSION << std::endl;
		accumulator << "Compiler: " << ELPIDA_COMPILER_NAME << " Version: " << ELPIDA_COMPILER_VERSION << std::endl;

		return accumulator.str();
	}

	static const char* GetValueOrDefault(const char* value)
	{
		return value ? value : "";
	}

	bool ArgumentsHelper::ParseAndGetExitText(int argC, char** argV, std::string& returnText)
	{
		enum Flags
		{
			Version,
			Help,
			Format,
			Delay,
			Upload,
			Report,
			Iterations,
			BenchmarksDirectory,
			ResultPath,
			Quiet
		};

		struct option options[] = {
				{ "version", no_argument, nullptr, Version },
				{ "help", no_argument, nullptr, Help },
				{ "format", required_argument, nullptr, Format },
				{ "delay-seconds", required_argument, nullptr, Delay },
				{ "upload", no_argument, nullptr, Upload },
				{ "generate-report", optional_argument, nullptr, Report },
				{ "iterations", required_argument, nullptr, Iterations },
				{ "benchmarks-directory", required_argument, nullptr, BenchmarksDirectory },
				{ "output", required_argument, nullptr, ResultPath },
				{ "quiet", no_argument, nullptr, Quiet },
				{ nullptr, 0, nullptr, 0 }
		};

		int option_index = 0;
		int c;
		while ((c = getopt_long(argC, argV, "vhf:d:ur:i:b:qo:", options, &option_index)) != -1)
		{
			switch (c)
			{
			case 'v':
			case Version:
				returnText = GetVersionString();
				return true;
			case 'h':
			case Help:
				returnText = GetHelpString();
				return true;
			case 'f':
			case Format:
				ParseFormat(GetValueOrDefault(optarg));
				break;
			case 'd':
			case Delay:
				_secondsBetweenRuns = ParseUnsigned(GetValueOrDefault(optarg), "delay-seconds");
				break;
			case 'u':
			case Upload:
				_upload = true;
				break;
			case 'r':
			case Report:
				_generateReport = true;
				_reportPath = GetValueOrDefault(optarg);
				break;
			case 'i':
			case Iterations:
				_iterationsToRun = ParseUnsigned(GetValueOrDefault(optarg), "iterations");
				break;
			case 'b':
			case BenchmarksDirectory:
				_benchmarksPath = GetValueOrDefault(optarg);
				if (_benchmarksPath.empty())
				{
					returnText = "'benchmarks-directory' cannot be empty.";
					return false;
				}
				break;
			case 'o':
			case ResultPath:
				_resultPath = GetValueOrDefault(optarg);
				if (_resultPath.empty())
				{
					returnText = "'results-output' cannot be empty.";
					return false;
				}
				break;
			case 'q':
			case Quiet:
				_quiet = true;
				break;
			case '?':
				returnText = "Unknown option: " + std::string(GetValueOrDefault(optarg));
				return false;
			default:
				break;
			}
		}

		return true;
	}

	void ArgumentsHelper::ParseFormat(const String& value)
	{
		if (value == "json")
		{
			_outputFormat = OutputFormat::Json;
		}
	}

	ArgumentsHelper::ArgumentsHelper()
			:_reportPath("./Elpida-Benchmark-Report.html"), _generateReport(false), _upload(false),
			 _outputFormat(OutputFormat::Default), _secondsBetweenRuns(0), _iterationsToRun(1), _quiet(false)
	{

	}

	const std::string& ArgumentsHelper::GetBenchmarksPath() const
	{
		return _benchmarksPath;
	}

	const std::string& ArgumentsHelper::GetReportPath() const
	{
		return _reportPath;
	}

	size_t ArgumentsHelper::GetIterationsToRun() const
	{
		return _iterationsToRun;
	}

	std::size_t ArgumentsHelper::GetSecondsBetweenRuns() const
	{
		return _secondsBetweenRuns;
	}

	OutputFormat ArgumentsHelper::GetOutputFormat() const
	{
		return _outputFormat;
	}

	bool ArgumentsHelper::IsGenerateReport() const
	{
		return _generateReport;
	}

	bool ArgumentsHelper::IsUpload() const
	{
		return _upload;
	}

	const std::string& ArgumentsHelper::GetResultPath() const
	{
		return _resultPath;
	}

} // Elpida