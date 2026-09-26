/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 12/3/2023.
//

#include "ArgumentsHelper.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Platform/OsUtilities.hpp"

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
		accumulator << R"(Elpida Info Dumper: )" << ELPIDA_VERSION << std::endl;
		accumulator << R"(Example usage: elpida-info-dumper --format=json --upload ...)"<< std::endl;
		accumulator << R"(       -v, --version)" << std::endl;
		accumulator << R"(           Prints the version and exits)" << std::endl;
		accumulator << R"(       -h, --help)" << std::endl;
		accumulator << R"(           Prints this help and exits)" << std::endl;
		accumulator << R"(       --benchmarks-directory=DIRECTORY)" << std::endl;
		accumulator << R"(           Where to find the benchmarks (Default is './Benchmarks'))" << std::endl;
		accumulator << R"(       --benchmarks-suffix=SUFFIX)" << std::endl;
		accumulator << R"(           The suffix (including any extension) of the benchmark executables to search. (default = "" = any executable in directory))" << std::endl;
		accumulator << R"(       --no-thread-pinning)" << std::endl;
		accumulator << R"(           Do not attempt to pin threads when calculating timing)" << std::endl;


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
			BenchmarksDirectory,
			BenchmarkSuffix,
			NoThreadPinning,
		};

		struct option options[] = {
				{ "version", no_argument, nullptr, Version },
				{ "help", no_argument, nullptr, Help },
				{ "benchmarks-suffix", required_argument, nullptr, BenchmarkSuffix },
				{ "benchmarks-directory", required_argument, nullptr, BenchmarksDirectory },
				{ "no-thread-pinning", no_argument, nullptr, NoThreadPinning },
				{ nullptr, 0, nullptr, 0 }
		};

		int option_index = 0;
		int c;
		while ((c = getopt_long(argC, argV, "vhb:s:n", options, &option_index)) != -1)
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
			case 'b':
			case BenchmarksDirectory:
				_benchmarksPath = GetValueOrDefault(optarg);
				if (_benchmarksPath.empty())
				{
					returnText = "'benchmarks-directory' cannot be empty.";
					return false;
				}
				break;
			case 's':
			case BenchmarkSuffix:
				_benchmarkSuffix = GetValueOrDefault(optarg);
				if (_benchmarkSuffix.empty())
				{
					returnText = "'benchmarks-suffix' cannot be empty.";
					return false;
				}
				break;
			case 'n':
			case NoThreadPinning:
				_noThreadPinning = true;
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

	ArgumentsHelper::ArgumentsHelper()
			: _noThreadPinning(false)
	{
		_benchmarksPath = OsUtilities::GetExecutableDirectory() / "Benchmarks";
	}

	const std::filesystem::path& ArgumentsHelper::GetBenchmarksPath() const
	{
		return _benchmarksPath;
	}

	const std::string& ArgumentsHelper::GetBenchmarkSuffix() const
	{
		return _benchmarkSuffix;
	}

	bool ArgumentsHelper::IsNoThreadPinning() const
	{
		return _noThreadPinning;
	}
} // Elpida