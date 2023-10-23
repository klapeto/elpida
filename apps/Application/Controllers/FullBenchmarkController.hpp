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
	class TopologyModel;
	class BenchmarkExecutionService;
	class TimingModel;

	class FullBenchmarkController : public Controller<FullBenchmarkModel>
	{
	public:
		Promise<> RunAsync();
		void StopRunning();
		FullBenchmarkController(FullBenchmarkModel& model,
				const TopologyModel& topologyModel,
				const TimingModel& overheadsModel,
				BenchmarkExecutionService& benchmarkExecutionService,
				const std::vector<BenchmarkGroupModel>& benchmarkGroups);

		~FullBenchmarkController() override = default;
	private:
		const TopologyModel& _topologyModel;
		const TimingModel& _overheadsModel;
		BenchmarkExecutionService& _benchmarkExecutionService;

		const BenchmarkModel* _memoryLatency;
		const BenchmarkModel* _memoryReadBandwidth;
		const BenchmarkModel* _pngEncoding;
		bool _cancelling;
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKCONTROLLER_HPP
