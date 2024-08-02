//
// Created by klapeto on 12/3/2023.
//

#include "Elpida/EntryPoint/ArgumentsHelper.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/EntryPoint/DefaultFormatter.hpp"
#include "Elpida/EntryPoint/JsonFormatter.hpp"

#include <filesystem>

#ifdef _MSC_VER
#include "getopt_MSVC.h"
#else
#include <getopt.h>
#endif

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
		accumulator << R"("       --now-overhead=SECONDS)" << std::endl;
		accumulator << R"("           The now overhead in fractional seconds)" << std::endl;
		accumulator << R"("       --loop-overhead=SECONDS)" << std::endl;
		accumulator << R"("           The loop overhead in fractional seconds)" << std::endl;
		accumulator << R"("       --numa-aware)" << std::endl;
		accumulator << R"("           Enable numa aware allocations)" << std::endl;
		accumulator << R"("       --pin-threads)" << std::endl;
		accumulator
				<< R"("           Enable thread pining. Threads will be pinned to target affinity and cannot jump to other processors)"
				<< std::endl;
		accumulator << R"("       --concurrency-mode=MODE)" << std::endl;
		accumulator << R"("           The concurrency to use on tasks. Can be 'None' or '0' (default), 'CopyInput' or '1', 'ShareInput' or '2', 'ChunkInput' or '3')" << std::endl;
		accumulator << R"("       --dump-info)" << std::endl;
		accumulator << R"("           Dumps the benchmark data of this benchmark)" << std::endl;

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

		while (!str.empty() && str[0] == '"')
		{
			str = str.substr(1);
		}

		while (!str.empty() && str[str.length() - 1] == '"')
		{
			str = str.substr(0, str.length() - 1);
		}
		return str;
	}

	static ConcurrencyMode ParseConcurrencyMode(const String& value)
	{
		if (value.empty())
		{
			throw ElpidaException("'--concurrency-mode' option cannot be empty");
		}

		try
		{
			auto actualValue = std::stol(value);
			if (actualValue ==static_cast<std::size_t>(ConcurrencyMode::None)) return ConcurrencyMode::None;
			if (actualValue ==static_cast<std::size_t>(ConcurrencyMode::CopyInput)) return ConcurrencyMode::CopyInput;
			if (actualValue ==static_cast<std::size_t>(ConcurrencyMode::ShareInput)) return ConcurrencyMode::ShareInput;
			if (actualValue ==static_cast<std::size_t>(ConcurrencyMode::ChunkInput)) return ConcurrencyMode::ChunkInput;
			throw ElpidaException("'--concurrency-mode' option must be 'None' or '0' (default), 'CopyInput' or '1', 'ShareInput' or '2', 'ChunkInput' or '4'");
		}
		catch (const std::invalid_argument& e)
		{
		}
		catch (const std::out_of_range& e)
		{
		}

		if (value == "None") return ConcurrencyMode::None;
		if (value == "CopyInput") return ConcurrencyMode::CopyInput;
		if (value == "ShareInput") return ConcurrencyMode::ShareInput;
		if (value == "ChunkInput") return ConcurrencyMode::ChunkInput;

		throw ElpidaException("'--concurrency-mode' option must be 'None' or '0' (default), 'CopyInput' or '1', 'ShareInput' or '2', 'ChunkInput' or '4'");
	}

	double static ParseDouble(const String& value, const char* name)
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
			NumaAware,
			PinThreads,
			ConcurrencyMode,
			DumpInfo
		};

		struct option options[] = {
				{ "version",             no_argument,       nullptr, Version },
				{ "help",                no_argument,       nullptr, Help },
				{ "module",              required_argument, nullptr, Module },
				{ "index",               required_argument, nullptr, Index },
				{ "affinity",            required_argument, nullptr, Affinity },
				{ "format",              required_argument, nullptr, Format },
				{ "config",              required_argument, nullptr, Config },
				{ "now-overhead",     required_argument, nullptr, NowOverhead },
				{ "loop-overhead",    required_argument, nullptr, LoopOverhead },
				{ "numa-aware",          no_argument,       nullptr, NumaAware },
				{ "pin-threads",         no_argument,       nullptr, PinThreads },
				{ "concurrency-mode", required_argument, nullptr, ConcurrencyMode },
				{ "dump-info", no_argument, nullptr, DumpInfo },
				{ nullptr, 0,                               nullptr, 0 }
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
				_nowOverhead = ParseDouble(GetValueOrDefault(optarg), "now-overhead");
				break;
			case LoopOverhead:
				_loopOverhead = ParseDouble(GetValueOrDefault(optarg), "loop-overhead");
				break;
			case NumaAware:
				_numaAware = true;
				break;
			case PinThreads:
				_pinThreads = true;
				break;
			case DumpInfo:
				_dumpInfo = true;
				return true;
			case ConcurrencyMode:
				_concurrencyMode = ParseConcurrencyMode(GetValueOrDefault(optarg));
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

	ArgumentsHelper::ArgumentsHelper()
			:_benchmarkIndex(0), _nowOverhead(0), _loopOverhead(0),
			 _concurrencyMode(ConcurrencyMode::None),
			 _numaAware(false), _pinThreads(false), _dumpInfo(false)
	{

	}

	bool ArgumentsHelper::GetNumaAware() const
	{
		return _numaAware;
	}

	bool ArgumentsHelper::GetPinThreads() const
	{
		return _pinThreads;
	}

	ConcurrencyMode ArgumentsHelper::GetConcurrencyMode() const
	{
		return _concurrencyMode;
	}

	bool ArgumentsHelper::GetDumpInfo() const
	{
		return _dumpInfo;
	}

} // Elpida