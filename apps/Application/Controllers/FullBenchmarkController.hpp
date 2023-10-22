//
// Created by klapeto on 22/10/2023.
//

#ifndef ELPIDA_FULLBENCHMARKCONTROLLER_HPP
#define ELPIDA_FULLBENCHMARKCONTROLLER_HPP


#include "Models/Full/FullBenchmarkModel.hpp"
#include "Controller.hpp"

namespace Elpida::Application
{
	class TopologyModel;
	class BenchmarkExecutionService;
	class TimingModel;

	class FullBenchmarkController : public Controller<FullBenchmarkModel>
	{
	public:
		void Run();
		void StopRunning();
		FullBenchmarkController(FullBenchmarkModel& model, const TopologyModel& topologyModel,
				const TimingModel& overheadsModel, BenchmarkExecutionService& benchmarkExecutionService);

		~FullBenchmarkController() override = default;
	private:
		const TopologyModel& _topologyModel;
		const TimingModel& _overheadsModel;
		BenchmarkExecutionService& _benchmarkExecutionService;
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKCONTROLLER_HPP
