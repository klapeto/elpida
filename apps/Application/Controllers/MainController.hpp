//
// Created by klapeto on 3/5/2024.
//

#ifndef ELPIDA_MAINCONTROLLER_HPP
#define ELPIDA_MAINCONTROLLER_HPP

#include <filesystem>

namespace Elpida::Application
{
	class FullBenchmarkModel;
	class CustomBenchmarkResultsModel;
	class ResultSerializer;

	class MainController
	{
	public:
		void SaveFullResults(const std::filesystem::path& filePath) const;
		void SaveCustomResults(const std::filesystem::path& filePath) const;

		explicit MainController(const FullBenchmarkModel& fullBenchmarkModel,
				const CustomBenchmarkResultsModel& customBenchmarkResultsModel,
				const ResultSerializer& resultSerializer);
	private:
		const FullBenchmarkModel& _fullBenchmarkModel;
		const CustomBenchmarkResultsModel& _customBenchmarkResultsModel;
		const ResultSerializer& _resultSerializer;
	};

} // Application
// Elpida

#endif //ELPIDA_MAINCONTROLLER_HPP
