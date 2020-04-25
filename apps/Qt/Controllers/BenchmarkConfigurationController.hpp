//
// Created by klapeto on 23/4/20.
//

#ifndef APPS_QT_CONTROLLERS_BENCHMARKCONFIGURATIONCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_BENCHMARKCONFIGURATIONCONTROLLER_HPP

#include "Core/Abstractions/CommandHandler.hpp"

namespace Elpida {
	class BenchmarkConfigurationsCollectionModel;
	class BenchmarkConfigurationModel;

	class BenchmarkConfigurationController: public CommandHandler
	{
	public:

		void handle(SelectedBenchmarkChangedEvent &command) override;

		BenchmarkConfigurationController(BenchmarkConfigurationsCollectionModel& benchmarkConfigurationsCollectionModel,
			BenchmarkConfigurationModel& configurationModel);
		~BenchmarkConfigurationController() override =default;
	private:
		BenchmarkConfigurationsCollectionModel& _benchmarkConfigurationsCollectionModel;
		BenchmarkConfigurationModel& _configurationModel;
	};
}



#endif //APPS_QT_CONTROLLERS_BENCHMARKCONFIGURATIONCONTROLLER_HPP
