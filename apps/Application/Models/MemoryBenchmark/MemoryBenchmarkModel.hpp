//
// Created by klapeto on 18/6/2024.
//

#ifndef ELPIDA_MEMORYBENCHMARKMODEL_HPP
#define ELPIDA_MEMORYBENCHMARKMODEL_HPP

#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Models/MemoryBenchmark/MemoryBenchmarkResultModel.hpp"

namespace Elpida::Application
{

	enum class MemoryBenchmarkType
	{
		MemoryLatency,
		MemoryReadBandwidth
	};

	class MemoryBenchmarkModel : public ListModel<MemoryBenchmarkResultModel>
	{
	public:
		bool IsRunning() const;
		Event<bool>& RunningChanged() const;
		Event<const std::string&>& CurrentRunningBenchmarkChanged() const;

		void SetRunning(bool running);
		void SetTotalBenchmarks(size_t totalBenchmarks);
		void SetCurrentRunningBenchmark(const std::string& currentRunningBenchmark);
		void SetIterations(size_t iterations);
		void SetInitialBytes(size_t initialBytes);
		void SetMultiplier(double multiplier);
		MemoryBenchmarkType GetBenchmarkType() const;
		void SetBenchmarkType(MemoryBenchmarkType benchmarkType);

		size_t GetTotalBenchmarks() const;
		size_t GetInitialBytes() const;
		double GetMultiplier() const;
		std::size_t GetIterations() const;

		MemoryBenchmarkModel();
	private:
		mutable Event<bool> _runningChanged;
		mutable Event<const std::string&> _currentRunningBenchmarkChanged;

		std::string _currentRunningBenchmark;
		MemoryBenchmarkType _benchmarkType;
		std::size_t _totalBenchmarks;
		std::size_t _initialBytes;
		double _multiplier;
		std::size_t _iterations;
		bool _running;
	};

} // Application
// Elpida

#endif //ELPIDA_MEMORYBENCHMARKMODEL_HPP
