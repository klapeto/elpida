//
// Created by klapeto on 20/10/2023.
//

#ifndef ELPIDA_BENCHMARKRUNCONFIGURATIONCONTROLLER_HPP
#define ELPIDA_BENCHMARKRUNCONFIGURATIONCONTROLLER_HPP

#include "Controller.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"


namespace Elpida::Application
{

	class BenchmarkRunConfigurationController final: public Controller<BenchmarkRunConfigurationModel>
	{
	public:
		void SetUploadResults(bool uploadResults);
		void SetOpenResultAfterUpload(bool openResult);
		void SetIterationsToRun(std::size_t iterationsToRun);

		explicit BenchmarkRunConfigurationController(BenchmarkRunConfigurationModel& model);
		~BenchmarkRunConfigurationController() = default;
	};

} // Elpida
// Application

#endif //ELPIDA_BENCHMARKRUNCONFIGURATIONCONTROLLER_HPP
