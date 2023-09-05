//
// Created by klapeto on 12/3/2023.
//

#include "ArgumentsHelper.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "DefaultFormatter.hpp"
#include "JsonFormatter.hpp"

#include <filesystem>
#include <getopt.h>

namespace Elpida
{
	void ArgumentsHelper::ValidateAffinity()
	{
		for (Size i = 0; i < _affinity.size(); ++i)
		{
			auto value = _affinity[i];
			for (Size j = i + 1; j < _affinity.size(); ++j)
			{
				if (_affinity[j] == value)
				{
					throw ElpidaException("Duplicate affinity '", value, "' detected. Only unique processors allowed");
				}
			}
		}
	}

	Size ArgumentsHelper::GetBenchmarkIndex() const
	{
		return _benchmarkIndex;
	}

	const ResultFormatter& ArgumentsHelper::GetResultFormatter() const
	{
		return *_resultFormatter;
	}

	void ArgumentsHelper::ParseAffinity(const String& value)
	{
		std::ostringstream accumulator;
		for (Size i = 0; i < value.size(); ++i)
		{
			auto c = value[i];

			if (c == ',')
			{
				auto str = accumulator.str();
				_affinity.push_back(std::stoul(str));
				accumulator.str(std::string());
			}
			else if (std::isdigit(c))
			{
				accumulator << c;
			}
			else
			{
				throw ElpidaException("unexpected character '", (char)c, "' while parsing affinity at pos: ", i);
			}
		}

		auto str = accumulator.str();
		_affinity.push_back(std::stoul(str));
	}

	String ArgumentsHelper::GetHelpString()
	{
		std::ostringstream accumulator;
		accumulator << R"("Elpida Benchmark Executor: )" << ELPIDA_VERSION << std::endl;
		accumulator
			<< R"(Example usage: elpida-executor --module="./dir/benchmark.so" --index=0 --affinity=0,1,5,32 --format=json --config="Config A" --config="Config B" ...)"
			<< std::endl;
		accumulator << R"("       -v, --version)" << std::endl;
		accumulator << R"("           Prints the version and exits)" << std::endl;
		accumulator << R"("       -h, --help)" << std::endl;
		accumulator << R"("           Prints this help and exits)" << std::endl;
		accumulator << R"("       --module="MODULE_PATH")" << std::endl;
		accumulator << R"("           The library that contains the benchmark group)" << std::endl;
		accumulator << R"("       --index=BENCHMARK_INDEX)" << std::endl;
		accumulator << R"("           The index of the benchmark in the benchmark group)" << std::endl;
		accumulator << R"("       --affinity=AFFINITY)" << std::endl;
		accumulator << R"("           The comma separated processors to use.)" << std::endl;
		accumulator << R"("       --format=FORMAT)" << std::endl;
		accumulator << R"("           The result output format. Accepted values (default, json))" << std::endl;
		accumulator << R"("       --config="CONFIG_VALUE")" << std::endl;
		accumulator
			<< R"("           Sets a configuration. Successive configurations are appended in the order defined)"
			<< std::endl;
		accumulator << R"("       --now-nanoseconds=NANOSECONDS)" << std::endl;
		accumulator << R"("           The now overhead in nanoseconds)" << std::endl;
		accumulator << R"("       --loop-nanoseconds=NANOSECONDS)" << std::endl;
		accumulator << R"("           The loop overhead in nanoseconds)" << std::endl;
		accumulator << R"("       --virtual-nanoseconds=NANOSECONDS)" << std::endl;
		accumulator << R"("           The virtual call overhead in nanoseconds)" << std::endl;

		return accumulator.str();
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

	static std::string GetConfigValue(const char* value)
	{
		if (!value) return {};
		std::string str(value);

		while (str.starts_with('\"'))
		{
			str = str.substr(1);
		}

		while (str.ends_with('\"'))
		{
			str = str.substr(0, str.length() - 1);
		}
		return str;
	}

	double static ParseTime(const String& value, const char* name)
	{
		if (value.empty())
		{
			throw ElpidaException("'--", name, "' option cannot be empty");
		}
		try
		{
			auto actualValue = std::stod(value);
			if (actualValue < 0.0)
			{
				throw ElpidaException("'--", name, "' option cannot be negative");
			}

			return actualValue;
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

	bool ArgumentsHelper::ParseAndGetExitText(int argC, char** argV, std::string& returnText)
	{
		enum Flags
		{
			Version,
			Help,
			Module,
			Index,
			Affinity,
			Format,
			Config,
			NowOverhead,
			LoopOverhead,
			VirtualOverhead
		};

		struct option options[] = {
			{ "version", no_argument, nullptr, Version },
			{ "help", no_argument, nullptr, Help },
			{ "module", required_argument, nullptr, Module },
			{ "index", required_argument, nullptr, Index },
			{ "affinity", required_argument, nullptr, Affinity },
			{ "format", required_argument, nullptr, Format },
			{ "config", required_argument, nullptr, Config },
			{ "now-nanoseconds", required_argument, nullptr, NowOverhead },
			{ "loop-nanoseconds", required_argument, nullptr, LoopOverhead },
			{ "virtual-nanoseconds", required_argument, nullptr, VirtualOverhead },
			{ nullptr, 0, nullptr, 0 }
		};

		int option_index = 0;
		int c;
		while ((c = getopt_long(argC, argV, "vhm:i:a:f:c:", options, &option_index)) != -1)
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
			case Module:
				ParseModulePath(GetValueOrDefault(optarg));
				break;
			case Index:
				ParseIndex(GetValueOrDefault(optarg));
				break;
			case Affinity:
				ParseAffinity(GetValueOrDefault(optarg));
				break;
			case Format:
				ParseFormat(GetValueOrDefault(optarg));
				break;
			case Config:
				_configurationValues.emplace_back(GetConfigValue(optarg));
				break;
			case NowOverhead:
				_nowOverhead = ParseTime(GetValueOrDefault(optarg), "now-overhead");
				break;
			case LoopOverhead:
				_loopOverhead = ParseTime(GetValueOrDefault(optarg), "loop-overhead");
				break;
			case VirtualOverhead:
				_vCallOverhead = ParseTime(GetValueOrDefault(optarg), "virtual-overhead");
				break;
			case '?':
				returnText = "Unknown option: " + std::string(GetValueOrDefault(optarg));
				return false;
			default:
				break;
			}
		}

		ValidateAffinity();

		if (!_resultFormatter)
		{
			_resultFormatter = std::make_unique<DefaultFormatter>();
		}

		return true;
	}

	void ArgumentsHelper::ParseFormat(const String& value)
	{
		if (value == "json")
		{
			_resultFormatter = std::make_unique<JsonFormatter>();
		}
		else
		{
			if (!_resultFormatter)
			{
				_resultFormatter = std::make_unique<DefaultFormatter>();
			}
		}
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

	void ArgumentsHelper::ParseIndex(const String& value)
	{
		_benchmarkIndex = ParseUnsigned(value, "index");
	}

	void ArgumentsHelper::ParseModulePath(const String& value)
	{
		if (value.empty())
		{
			throw ElpidaException("'--module' option cannot be empty");
		}
		auto actualPath = ValueUtilities::Trim(value, '"');

		if (!std::filesystem::exists(actualPath))
		{
			throw ElpidaException("module '", value, "' does not exist");
		}

		_modulePath = actualPath;
	}

	const String& ArgumentsHelper::GetModulePath() const
	{
		return _modulePath;
	}

	const Vector<unsigned int>& ArgumentsHelper::GetAffinity() const
	{
		return _affinity;
	}

	const Vector<String>& ArgumentsHelper::GetConfigurationValues() const
	{
		return _configurationValues;
	}

	double ArgumentsHelper::GetNowOverhead() const
	{
		return _nowOverhead;
	}

	double ArgumentsHelper::GetLoopOverhead() const
	{
		return _loopOverhead;
	}

	double ArgumentsHelper::GetVCallOverhead() const
	{
		return _vCallOverhead;
	}

	ArgumentsHelper::ArgumentsHelper()
		: _benchmarkIndex(0), _nowOverhead(0), _loopOverhead(0), _vCallOverhead(0)
	{

	}
} // Elpida