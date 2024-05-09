//
// Created by klapeto on 22/10/2023.
//

#ifndef ELPIDA_FULLBENCHMARKCONTROLLER_HPP
#define ELPIDA_FULLBENCHMARKCONTROLLER_HPP

#include "Models/Full/FullBenchmarkModel.hpp"
#include "Controller.hpp"

#include <thread>
#include <atomic>
#include <filesystem>

namespace Elpida::Application
{
	class BenchmarkExecutionService;
	class TimingModel;
	class ResultSerializer;
	class MessageService;
	class BenchmarkRunConfigurationModel;
	class TopologyModel;

	class FullBenchmarkController : public Controller<FullBenchmarkModel>
	{
	public:
		void RunAsync();
		void StopRunning();
		void ClearResults();
		void SaveResults(const std::filesystem::path& filePath);
		FullBenchmarkController(FullBenchmarkModel& model,
				const TimingModel& timingModel,
				const TopologyModel& topologyModel,
				const BenchmarkRunConfigurationModel& runConfigurationModel,
				BenchmarkExecutionService& benchmarkExecutionService,
				const ResultSerializer& resultSerializer,
				MessageService& messageService,
				const std::vector<BenchmarkGroupModel>& benchmarkGroups);

		~FullBenchmarkController() override;
	private:
		const TimingModel& _timingModel;
		const TopologyModel& _topologyModel;
		const BenchmarkRunConfigurationModel& _runConfigurationModel;
		BenchmarkExecutionService& _benchmarkExecutionService;
		const ResultSerializer& _resultSerializer;
		MessageService& _messageService;
		std::thread _runnerThread;
		std::atomic<bool> _running;
		std::atomic<bool> _cancelling;

		const BenchmarkModel* _memoryLatency;
		const BenchmarkModel* _memoryReadBandwidth;
		const BenchmarkModel* _svgRasterization;
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKCONTROLLER_HPP
