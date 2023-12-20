//
// Created by klapeto on 22/10/2023.
//

#ifndef ELPIDA_FULLBENCHMARKMODEL_HPP
#define ELPIDA_FULLBENCHMARKMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include "Models/Benchmark/BenchmarkGroupModel.hpp"
#include "FullBenchmarkResultModel.hpp"

namespace Elpida::Application
{

	class FullBenchmarkModel : public Model
	{
	public:
		const std::string& GetCurrentRunningBenchmark() const;
		size_t GetTotalBenchmarks() const;
		bool IsRunning() const;
		const std::vector<FullBenchmarkResultModel>& GetResults() const;

		Event<bool>& RunningChanged() const;
		Event<const std::string&>& CurrentRunningBenchmarkChanged() const;

		void SetCurrentRunningBenchmark(const std::string& benchmark);
		void SetRunning(bool running);
		void SetTotalBenchmarks(std::size_t totalBenchmarks);

		void AddResult(FullBenchmarkResultModel&& result);

		FullBenchmarkModel();
		~FullBenchmarkModel() override = default;
	private:
		std::vector<FullBenchmarkResultModel> _results;
		std::string _currentRunningBenchmark;
		std::size_t _totalBenchmarks;
		bool _running;
		mutable Event<bool> _runningChanged;
		mutable Event<const std::string&> _currentRunningBenchmarkChanged;
	};

} // Elpida
// Application

#endif //ELPIDA_FULLBENCHMARKMODEL_HPP