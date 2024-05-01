//
// Created by klapeto on 22/10/2023.
//

#ifndef ELPIDA_FULLBENCHMARKCONTROLLER_HPP
#define ELPIDA_FULLBENCHMARKCONTROLLER_HPP

#include "Models/Full/FullBenchmarkModel.hpp"
#include "Controller.hpp"
#include "Core/Promise.hpp"

namespace Elpida::Application
{
	class BenchmarkExecutionService;
	class TimingModel;
	class MessageService;
	class BenchmarkRunConfigurationModel;

	class FullBenchmarkController : public Controller<FullBenchmarkModel>
	{
	public:
		Promise<> RunAsync();
		void StopRunning();
		FullBenchmarkController(FullBenchmarkModel& model,
				const TimingModel& overheadsModel,
				const BenchmarkRunConfigurationModel& runConfigurationModel,
				BenchmarkExecutionService& benchmarkExecutionService,
				MessageService& messageService,
				const std::vector<BenchmarkGroupModel>& benchmarkGroups);

		~FullBenchmarkController() override = default;
	private:
		const TimingModel& _overheadsModel;
		const BenchmarkRunConfigurationModel& _runConfigurationModel;
		BenchmarkExecutionService& _benchmarkExecutionService;
		MessageService& _messageService;

		const BenchmarkModel* _memoryLatency;
		const BenchmarkModel* _memoryReadBandwidth;
		const BenchmarkModel* _svgRasterizationSingle;
		const BenchmarkModel* _svgRasterizationMulti;
		bool _cancelling;
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKCONTROLLER_HPP
