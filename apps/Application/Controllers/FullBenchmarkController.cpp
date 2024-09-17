//
// Created by klapeto on 22/10/2023.
//

#include "FullBenchmarkController.hpp"

#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Core/BenchmarkExecutionService.hpp"
#include "Core/MessageService.hpp"
#include "Core/ResultsHTMLReporter.hpp"
#include "Core/PathsService.hpp"
#include "Core/DesktopService.hpp"
#include "ResultSerializer.hpp"
#include "DataUploader.hpp"

#include "FullBenchmarkInstances/SvgRasterizationSingleThread.hpp"
#include "FullBenchmarkInstances/SvgRasterizationMultiThread.hpp"
#include "FullBenchmarkInstances/ParseJsonSingleThread.hpp"
#include "FullBenchmarkInstances/ParseJsonMultiThread.hpp"
#include "FullBenchmarkInstances/ZlibCompressionSingleThread.hpp"
#include "FullBenchmarkInstances/ZlibCompressionMultiThread.hpp"
#include "FullBenchmarkInstances/ZlibDecompressionSingleThread.hpp"
#include "FullBenchmarkInstances/ZlibDecompressionMultiThread.hpp"
#include "FullBenchmarkInstances/RSADecryptionMultiThread.hpp"
#include "FullBenchmarkInstances/RSADecryptionSingleThread.hpp"
#include "FullBenchmarkInstances/RSAEncryptionMultiThread.hpp"
#include "FullBenchmarkInstances/RSAEncryptionSingleThread.hpp"
#include "FullBenchmarkInstances/AESDecryptionMultiThread.hpp"
#include "FullBenchmarkInstances/AESDecryptionSingleThread.hpp"
#include "FullBenchmarkInstances/AESEncryptionMultiThread.hpp"
#include "FullBenchmarkInstances/AESEncryptionSingleThread.hpp"
#include "FullBenchmarkInstances/Base64EncodeMultiThread.hpp"
#include "FullBenchmarkInstances/Base64EncodeSingleThread.hpp"
#include "FullBenchmarkInstances/Base64DecodeMultiThread.hpp"
#include "FullBenchmarkInstances/Base64DecodeSingleThread.hpp"
#include "FullBenchmarkInstances/FFTMultiThread.hpp"
#include "FullBenchmarkInstances/FFTSingleThread.hpp"
#include "FullBenchmarkInstances/MatrixMultiplicationMultiThread.hpp"
#include "FullBenchmarkInstances/MatrixMultiplicationSingleThread.hpp"
#include "FullBenchmarkInstances/MatrixInverseMultiThread.hpp"
#include "FullBenchmarkInstances/MatrixInverseSingleThread.hpp"
#include "FullBenchmarkInstances/NBodyMultiThread.hpp"
#include "FullBenchmarkInstances/NBodySingleThread.hpp"
#include "FullBenchmarkInstances/RayTracingMultiThread.hpp"
#include "FullBenchmarkInstances/RayTracingSingleThread.hpp"
#include "FullBenchmarkInstances/RegexMultiThread.hpp"
#include "FullBenchmarkInstances/RegexSingleThread.hpp"
#include "FullBenchmarkInstances/XmlParsingMultiThread.hpp"
#include "FullBenchmarkInstances/XmlParsingSingleThread.hpp"

#include <sstream>
#include <fstream>
#include <locale>
#include <string>
#include <cmath>

namespace Elpida::Application
{
	static const double SingleCoreWeight = 1.1;
	static const double MultiCoreWeight = 1.0;

	FullBenchmarkController::FullBenchmarkController(FullBenchmarkModel& model,
			const TimingModel& timingModel,
			const TopologyModel& topologyModel,
			const MemoryInfoModel& memoryInfoModel,
			const BenchmarkRunConfigurationModel& runConfigurationModel,
			BenchmarkExecutionService& benchmarkExecutionService,
			const ResultSerializer& resultSerializer,
			const ResultsHTMLReporter& resultsHTMLReporter,
			const PathsService& pathsService,
			const DesktopService& desktopService,
			const DataUploader& dataUploader,
			MessageService& messageService,
			const std::vector<BenchmarkGroupModel>& benchmarkGroups)
			:
			Controller(model),
			_timingModel(timingModel),
			_topologyModel(topologyModel),
			_runConfigurationModel(runConfigurationModel),
			_benchmarkExecutionService(benchmarkExecutionService),
			_resultSerializer(resultSerializer),
			_resultsHTMLReporter(resultsHTMLReporter),
			_pathsService(pathsService),
			_desktopService(desktopService),
			_dataUploader(dataUploader),
			_messageService(messageService),
			_running(false),
			_cancelling(false)
	{
		for (auto& group : benchmarkGroups)
		{
			for (auto& benchmark : group.GetBenchmarks())
			{
				auto& name = benchmark.GetName();
				if (name == "Svg Rasterization")
				{
					_benchmarks.push_back(
							std::make_unique<SvgRasterizationSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<SvgRasterizationMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Json Parsing")
				{
					_benchmarks.push_back(
							std::make_unique<ParseJsonSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<ParseJsonMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Zlib compression")
				{
					_benchmarks.push_back(
							std::make_unique<ZlibCompressionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<ZlibCompressionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Zlib decompression")
				{
					_benchmarks.push_back(
							std::make_unique<ZlibDecompressionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<ZlibDecompressionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "RSA Encryption")
				{
					_benchmarks.push_back(
							std::make_unique<RSAEncryptionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<RSAEncryptionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "RSA Decryption")
				{
					_benchmarks.push_back(
							std::make_unique<RSADecryptionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<RSADecryptionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "AES Encryption")
				{
					_benchmarks.push_back(
							std::make_unique<AESEncryptionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<AESEncryptionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "AES Decryption")
				{
					_benchmarks.push_back(
							std::make_unique<AESDecryptionSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<AESDecryptionMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Base64 Encode")
				{
					_benchmarks.push_back(
							std::make_unique<Base64EncodeSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<Base64EncodeMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Base64 Decode")
				{
					_benchmarks.push_back(
							std::make_unique<Base64DecodeSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<Base64DecodeMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "FFT calculation in place")
				{
					_benchmarks.push_back(
							std::make_unique<FFTSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<FFTMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Matrix multiplication (32x32)")
				{
					_benchmarks.push_back(
							std::make_unique<MatrixMultiplicationSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<MatrixMultiplicationMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Matrix inverse (4x4)")
				{
					_benchmarks.push_back(
							std::make_unique<MatrixInverseSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<MatrixInverseMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "NBody Simulation")
				{
					_benchmarks.push_back(
							std::make_unique<NBodySingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<NBodyMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "3D Ray Tracing")
				{
					_benchmarks.push_back(
							std::make_unique<RayTracingSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<RayTracingMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "Regex")
				{
					_benchmarks.push_back(
							std::make_unique<RegexSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<RegexMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
				else if (name == "XML Parsing")
				{
					_benchmarks.push_back(
							std::make_unique<XmlParsingSingleThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
					_benchmarks.push_back(
							std::make_unique<XmlParsingMultiThread>(benchmark, timingModel, topologyModel,
									memoryInfoModel, benchmarkExecutionService));
				}
			}
		}

		if (_benchmarks.empty())
		{
			throw ElpidaException("Missing benchmarks");
		}

		_model.SetTotalBenchmarks(4);
	}

	void FullBenchmarkController::RunAsync()
	{
		if (_running.load(std::memory_order_acquire)) return;

		if (_runnerThread.joinable())
		{
			_runnerThread.join();
		}

		_running.store(true, std::memory_order_release);
		_cancelling.store(false, std::memory_order_release);

		_model.SetTotalBenchmarks(_benchmarks.size() * _runConfigurationModel.GetIterationsToRun());
		_runnerThread = std::thread([this]
		{
			_model.SetRunning(true);
			std::vector<FullBenchmarkResultModel> thisResults;
			thisResults.reserve(_runConfigurationModel.GetIterationsToRun());
			try
			{
				for (std::size_t i = 0; i < _runConfigurationModel.GetIterationsToRun(); ++i)
				{
					Score singleCoreScore = 0.0;
					Score multiCoreScore = 0.0;
					std::vector<BenchmarkResultModel> benchmarkResults;

					for (auto& benchmark : _benchmarks)
					{
						_model.SetCurrentRunningBenchmark(benchmark->GetName());

						auto result = benchmark->Run();
						result.GetBenchmarkResult().SetUuid(benchmark->GetUuid());
						singleCoreScore += result.GetSingleCoreScore();
						multiCoreScore += result.GetMultiThreadScore();
						benchmarkResults.push_back(std::move(result.GetBenchmarkResult()));
					}

					auto totalScore = CalculateTotalScore(singleCoreScore, multiCoreScore);

					auto result = FullBenchmarkResultModel(std::move(benchmarkResults), totalScore, singleCoreScore,
							multiCoreScore);
					_model.Add(result);
					thisResults.push_back(std::move(result));

					if (_cancelling.load(std::memory_order_acquire))
					{
						break;
					}

					if (_runConfigurationModel.GetDelaySecondsBetweenRuns() > 0
						&& _runConfigurationModel.GetIterationsToRun() > i + 1)
					{
						_model.SetCurrentRunningBenchmark("Waiting...");
						std::this_thread::sleep_for(Seconds(_runConfigurationModel.GetDelaySecondsBetweenRuns()));
					}

					if (_cancelling.load(std::memory_order_acquire))
					{
						break;
					}
				}

			}
			catch (const ElpidaException& ex)
			{
				if (!_cancelling.load(std::memory_order_acquire))
				{
					_messageService.ShowError("Failed to run benchmark: " + std::string(ex.what()));
				}
			}

			PostHandleResults(thisResults);

			_model.SetRunning(false);
			_running.store(false, std::memory_order_release);
		});
	}

	Score FullBenchmarkController::CalculateTotalScore(Score singleCoreScore, Score multiCoreScore)
	{
		return std::pow(singleCoreScore, SingleCoreWeight) + std::pow(multiCoreScore, MultiCoreWeight);
	}

	void FullBenchmarkController::PostHandleResults(const std::vector<FullBenchmarkResultModel>& thisResults) const
	{
		if (!thisResults.empty())
		{
			if (_runConfigurationModel.IsGenerateHtmlReport())
			{
				try
				{
					GenerateHtmlReport(thisResults);
				}
				catch (const std::exception& ex)
				{
					_messageService.ShowError(std::string("Failed to crate report: ") + ex.what());
				}
			}
			if (_runConfigurationModel.IsUploadResults())
			{
				try
				{
					_model.SetCurrentRunningBenchmark("Uploading...");
					auto url = _dataUploader.UploadResult(thisResults);
					if (_runConfigurationModel.IsOpenResult())
					{
						_desktopService.OpenUri(url);
					}
				}
				catch (const std::exception& ex)
				{
					_messageService.ShowError(ex.what());
				}
			}
		}
	}

	void FullBenchmarkController::GenerateHtmlReport(const std::vector<FullBenchmarkResultModel>& thisResults) const
	{
		try
		{
			std::string fileName = "Full Benchmark ";
			fileName
					.append(" ")
					.append(std::to_string(time(nullptr)))
					.append(".html");

			auto path = _pathsService.GetDownloadStoragePath() / "Elpida Exported Reports" / fileName;

			_resultsHTMLReporter.WriteFullBenchmarkReport(thisResults, path);

			_desktopService.OpenFile(path);
		}
		catch (const std::exception& ex)
		{
			_messageService.ShowError("Failed to create report: " + std::string(ex.what()));
		}
	}

	void FullBenchmarkController::StopRunning()
	{
		_cancelling.store(true, std::memory_order_release);
		_benchmarkExecutionService.StopCurrentExecution();
	}

	FullBenchmarkController::~FullBenchmarkController()
	{
		if (_runnerThread.joinable())
		{
			StopRunning();
			_runnerThread.join();
		}
	}

	void FullBenchmarkController::ClearResults()
	{
		_model.Clear();
	}

	void FullBenchmarkController::SaveResults(const std::filesystem::path& filePath)
	{
		std::fstream file(filePath.c_str(), std::ios::trunc | std::fstream::out);

		std::vector<FullBenchmarkResultModel> thisResults;
		thisResults.reserve(_model.Size());
		for (auto& result : _model.GetItems())
		{
			thisResults.push_back(result.GetValue());
		}
		file << _resultSerializer.Serialize(thisResults);
	}
} // Elpida
// Application