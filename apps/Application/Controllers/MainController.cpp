//
// Created by klapeto on 3/5/2024.
//

#include "MainController.hpp"
#include "ResultSerializer.hpp"
#include "Models/Full/FullBenchmarkModel.hpp"
#include "Models/Custom/CustomBenchmarkResultsModel.hpp"

#include <fstream>

namespace Elpida::Application
{
	MainController::MainController(const FullBenchmarkModel& fullBenchmarkModel,
			const CustomBenchmarkResultsModel& customBenchmarkResultsModel, const ResultSerializer& resultSerializer)
			:_fullBenchmarkModel(fullBenchmarkModel), _customBenchmarkResultsModel(customBenchmarkResultsModel),
			 _resultSerializer(resultSerializer)
	{
	}

	void MainController::SaveFullResults(const std::filesystem::path& filePath) const
	{
		std::fstream file(filePath.c_str(), std::ios::trunc | std::fstream::out);
		file << _resultSerializer.Serialize(_fullBenchmarkModel.GetResults());
	}

	void MainController::SaveCustomResults(const std::filesystem::path& filePath) const
	{
		std::fstream file(filePath.c_str(), std::ios::trunc | std::fstream::out);
		file << _resultSerializer.Serialize(_customBenchmarkResultsModel);
	}
} // Application
// Elpida