/*
 *  Copyright (c) 2026  Ioannis Panagiotopoulos
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

// elpida-affinity-test: runs the single-thread benchmarks of the full benchmark (the same instances, configurations and
// benchmark executables as elpida-cli) twice per repeat: pinned to one processor (--affinity, what the full benchmark did
// before), and with no affinity, so the OS schedules the thread (what it does now). Prints the results, their spread and
// the single-thread score of both. See docs/development/single-thread-affinity.md.

#include "InfoGetter.hpp"
#include "ModelBuilderJson.hpp"
#include "FullBenchmarkInstancesLoader.hpp"
#include "ScoreCalculator.hpp"
#include "Core/BenchmarkExecutionService.hpp"
#include "Core/OffProcessBenchmarkExecutionService.hpp"
#include "Controllers/FullBenchmarkInstance.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"
#include "Models/Benchmark/BenchmarkModel.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

using namespace Elpida;
using namespace Elpida::Application;

namespace
{
	// Forwards to the real execution service with the affinity of the current mode instead of the instance's
	class AffinityExecutionService final : public BenchmarkExecutionService
	{
	public:
		explicit AffinityExecutionService(BenchmarkExecutionService& inner)
				: _inner(inner)
		{
		}

		void SetAffinity(std::vector<std::size_t> affinity)
		{
			_affinity = std::move(affinity);
		}

		BenchmarkResultModel Execute(const BenchmarkModel& benchmarkModel,
				const std::vector<std::size_t>&,
				double nowOverheadSeconds,
				double loopOverheadSeconds,
				bool numaAware,
				bool pinThreads,
				ConcurrencyMode concurrencyMode,
				double minimumMicroTaskDuration) override
		{
			return _inner.Execute(benchmarkModel, _affinity, nowOverheadSeconds, loopOverheadSeconds, numaAware,
					pinThreads, concurrencyMode, minimumMicroTaskDuration);
		}

		void StopCurrentExecution() override
		{
			_inner.StopCurrentExecution();
		}

	private:
		BenchmarkExecutionService& _inner;
		std::vector<std::size_t> _affinity;
	};

	struct Options
	{
		std::size_t repeats = 3;
		std::optional<std::size_t> processor;
		std::filesystem::path benchmarksPath;
		std::string filter;
		std::filesystem::path csvPath;
		bool noDetect = false;
	};

	void PrintHelp()
	{
		std::cout
				<< "elpida-affinity-test: single-thread benchmarks pinned to one processor vs scheduled by the OS\n"
				<< "Usage: elpida-affinity-test [options]\n"
				<< "  --repeats=N              Runs per mode and benchmark (default 3). The modes alternate.\n"
				<< "  --processor=N            Processor to pin to (default: the info dumper's fastest processor)\n"
				<< "  --no-detect              Don't let the info dumper search for the fastest processor (it pins\n"
				<< "                           to every core); requires --processor\n"
				<< "  --filter=TEXT            Only benchmarks whose name contains TEXT\n"
				<< "  --benchmarks-directory=D Where the benchmark executables are (default ./Benchmarks)\n"
				<< "  --csv=FILE               Also write every run to FILE\n"
				<< "  -h, --help               Prints this help\n";
	}

	bool StartsWith(const std::string& text, const std::string& prefix)
	{
		return text.compare(0, prefix.size(), prefix) == 0;
	}

	Options ParseOptions(int argC, char** argV)
	{
		Options options;
		for (int i = 1; i < argC; ++i)
		{
			std::string arg = argV[i];
			auto value = [&](const std::string& prefix)
			{
				return arg.substr(prefix.size());
			};

			if (arg == "-h" || arg == "--help")
			{
				PrintHelp();
				std::exit(0);
			}
			else if (StartsWith(arg, "--repeats="))
			{
				options.repeats = std::stoul(value("--repeats="));
			}
			else if (StartsWith(arg, "--processor="))
			{
				options.processor = std::stoul(value("--processor="));
			}
			else if (arg == "--no-detect")
			{
				options.noDetect = true;
			}
			else if (StartsWith(arg, "--filter="))
			{
				options.filter = value("--filter=");
			}
			else if (StartsWith(arg, "--benchmarks-directory="))
			{
				options.benchmarksPath = ValueUtilities::PathFromUtf8(value("--benchmarks-directory="));
			}
			else if (StartsWith(arg, "--csv="))
			{
				options.csvPath = ValueUtilities::PathFromUtf8(value("--csv="));
			}
			else
			{
				throw std::runtime_error("Unknown argument: " + arg + " (see --help)");
			}
		}

		if (options.repeats == 0)
		{
			throw std::runtime_error("--repeats must be at least 1");
		}
		if (options.noDetect && !options.processor.has_value())
		{
			throw std::runtime_error("--no-detect requires --processor");
		}
		return options;
	}

	struct Stats
	{
		double mean = 0.0;
		double cv = 0.0;    // coefficient of variation, %
	};

	Stats GetStats(const std::vector<double>& values)
	{
		Stats stats;
		for (auto v : values) stats.mean += v;
		stats.mean /= static_cast<double>(values.size());
		if (values.size() > 1 && stats.mean != 0.0)
		{
			double sum = 0.0;
			for (auto v : values) sum += (v - stats.mean) * (v - stats.mean);
			stats.cv = std::sqrt(sum / static_cast<double>(values.size() - 1)) / stats.mean * 100.0;
		}
		return stats;
	}

	std::string Format(double value, int precision)
	{
		std::ostringstream stream;
		stream << std::fixed << std::setprecision(precision) << value;
		return stream.str();
	}
}

int main(int argC, char** argV)
{
	try
	{
		auto options = ParseOptions(argC, argV);
		if (options.benchmarksPath.empty())
		{
			options.benchmarksPath = OsUtilities::GetExecutableDirectory() / "Benchmarks";
		}

		std::cout << "Getting system information..." << std::endl;
		InfoGetter infoGetter;
		infoGetter.SetBenchmarksPath(options.benchmarksPath);
		infoGetter.SetNoThreadPinning(options.noDetect);
		ModelBuilderJson builderJson(infoGetter.GetData());

		for (auto& [file, reason] : builderJson.GetFailedToLoadBenchmarkGroups())
		{
			std::cerr << "Failed to load benchmark group " << file << ": " << reason << std::endl;
		}

		auto processor = options.processor.value_or(builderJson.GetTopologyInfoModel().GetFastestProcessor());

		OffProcessBenchmarkExecutionService realService;
		AffinityExecutionService service(realService);
		BenchmarkRunConfigurationModel runConfiguration;
		auto benchmarkGroups = builderJson.GetBenchmarkGroups();
		std::vector<std::string> missing;
		auto instances = FullBenchmarkInstancesLoader::Load(benchmarkGroups, builderJson.GetTimingModel(),
				builderJson.GetTopologyInfoModel(), builderJson.GetMemoryInfoModel(), runConfiguration, service,
				missing);

		std::vector<FullBenchmarkInstance*> selected;
		for (auto& instance : instances)
		{
			if (!instance->IsMultiThread()
				&& (options.filter.empty() || instance->GetName().find(options.filter) != std::string::npos))
			{
				selected.push_back(instance.get());
			}
		}
		if (selected.empty())
		{
			throw std::runtime_error("No single-thread benchmarks to run");
		}

		std::cout << "Pinned mode: --affinity=" << processor
				<< (options.processor.has_value() ? " (from --processor)" : " (the info dumper's fastest processor)")
				<< std::endl;
		std::cout << selected.size() << " benchmarks, " << options.repeats << " runs per mode, about "
				<< (selected.size() * options.repeats * 2 * 6 + 59) / 60 << " minutes" << std::endl << std::endl;

		std::ofstream csv;
		if (!options.csvPath.empty())
		{
			csv.open(options.csvPath);
			if (!csv)
			{
				throw std::runtime_error("Cannot write " + ValueUtilities::PathToUtf8(options.csvPath));
			}
			csv << "benchmark,unit,mode,run,result\n";
		}

		struct Row
		{
			std::string name;
			std::string unit;
			double baseScore;
			std::vector<double> pinned;
			std::vector<double> scheduled;
		};
		std::vector<Row> rows;

		for (auto* instance : selected)
		{
			Row row{ instance->GetName(), instance->GetBenchmark().GetResultUnit(), instance->GetBaseScore(), {}, {} };
			for (std::size_t run = 0; run < options.repeats; ++run)
			{
				// Alternate which mode goes first, so warm-up and heat affect both the same way
				for (int step = 0; step < 2; ++step)
				{
					bool pinned = (run % 2 == 0) == (step == 0);
					service.SetAffinity(pinned ? std::vector<std::size_t>{ processor } : std::vector<std::size_t>{});
					std::cout << "  " << row.name << " [" << (pinned ? "pinned" : "OS-scheduled") << " " << run + 1
							<< "/" << options.repeats << "]" << std::flush;
					auto result = instance->Run();
					auto value = result.GetScore();
					std::cout << " " << ValueUtilities::GetValueScaleStringSI(value) << row.unit << "/s" << std::endl;
					(pinned ? row.pinned : row.scheduled).push_back(value);
					if (csv)
					{
						csv << '"' << row.name << "\"," << row.unit << ',' << (pinned ? "pinned" : "scheduled") << ','
								<< run + 1 << ',' << std::setprecision(17) << value << '\n';
					}
				}
			}
			rows.push_back(std::move(row));
		}

		std::cout << std::endl << std::left << std::setw(40) << "Benchmark" << std::right << std::setw(24)
				<< ("Pinned (" + std::to_string(processor) + ")") << std::setw(7) << "CV%" << std::setw(24)
				<< "OS-scheduled" << std::setw(7) << "CV%" << std::setw(8) << "Diff%" << std::endl;

		std::vector<double> pinnedMeans, scheduledMeans, baseScores;
		for (auto& row : rows)
		{
			auto p = GetStats(row.pinned);
			auto s = GetStats(row.scheduled);
			pinnedMeans.push_back(p.mean);
			scheduledMeans.push_back(s.mean);
			baseScores.push_back(row.baseScore);
			std::cout << std::left << std::setw(40) << row.name << std::right << std::setw(24)
					<< ValueUtilities::GetValueScaleStringSI(p.mean) + row.unit + "/s" << std::setw(7)
					<< Format(p.cv, 1) << std::setw(24) << ValueUtilities::GetValueScaleStringSI(s.mean) + row.unit + "/s"
					<< std::setw(7) << Format(s.cv, 1) << std::setw(8)
					<< Format((s.mean - p.mean) / p.mean * 100.0, 1) << std::endl;
		}

		auto pinnedScore = ScoreCalculator::CalculateBenchmarkScore(pinnedMeans.data(), baseScores.data(),
				pinnedMeans.size());
		auto scheduledScore = ScoreCalculator::CalculateBenchmarkScore(scheduledMeans.data(), baseScores.data(),
				scheduledMeans.size());
		std::cout << std::endl << "Single-thread score: pinned " << Format(pinnedScore, 2) << ", OS-scheduled "
				<< Format(scheduledScore, 2) << " (" << Format((scheduledScore - pinnedScore) / pinnedScore * 100.0, 1)
				<< "%)" << std::endl;
		std::cout << "Diff% > 0: faster when the OS schedules. Compare it with the CV% (run-to-run spread): a "
				<< "difference smaller than the spread is noise." << std::endl;
		return 0;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}
}
