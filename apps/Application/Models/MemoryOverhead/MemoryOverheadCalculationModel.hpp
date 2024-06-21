//
// Created by klapeto on 18/6/2024.
//

#ifndef ELPIDA_MEMORYOVERHEADCALCULATIONMODEL_HPP
#define ELPIDA_MEMORYOVERHEADCALCULATIONMODEL_HPP

#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Models/MemoryOverhead/MemoryOverheadResultModel.hpp"

namespace Elpida::Application
{

	class MemoryOverheadCalculationModel : public ListModel<MemoryOverheadResultModel>
	{
	public:
		bool IsRunning() const;
		Event<bool>& RunningChanged() const;
		Event<const std::string&>& CurrentRunningBenchmarkChanged() const;

		size_t GetTotalBenchmarks() const;
		double GetSubSamplesMultiplier() const;
		size_t GetInitialSubSamples() const;
		double GetScaleMultiplier() const;
		double GetInitialScale() const;
		size_t GetIterations() const;

		void SetRunning(bool running);
		void SetTotalBenchmarks(size_t totalBenchmarks);
		void SetCurrentRunningBenchmark(const std::string& currentRunningBenchmark);
		void SetSubSamplesMultiplier(double subSamplesMultiplier);
		void SetInitialSubSamples(size_t initialSubSamples);
		void SetScaleMultiplier(double scaleMultiplier);
		void SetInitialScale(double initialScale);
		void SetIterations(size_t iterations);

		MemoryOverheadCalculationModel();
	private:
		std::string _currentRunningBenchmark;
		std::size_t _totalBenchmarks;
		bool _running;

		double _initialScale;
		double _scaleMultiplier;
		std::size_t _initialSubSamples;
		std::size_t _iterations;
		double _subSamplesMultiplier;
		mutable Event<bool> _runningChanged;
		mutable Event<const std::string&> _currentRunningBenchmarkChanged;
	};

} // Application
// Elpida

#endif //ELPIDA_MEMORYOVERHEADCALCULATIONMODEL_HPP
