//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_BENCHMARK_HPP
#define ELPIDA_BENCHMARK_HPP

#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/BenchmarkResult.hpp"
#include "Elpida/Core/BenchmarkInfo.hpp"
#include "Elpida/Core/Task.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Core/EnvironmentInfo.hpp"
#include "Elpida/Core/ConcurrencyMode.hpp"
#include "Elpida/Core/SharedPtr.hpp"

namespace Elpida {

	class TimingInfo;
	class BenchmarkRunContext;

	class Benchmark {
	public:
		[[nodiscard]]
		BenchmarkInfo GetInfo() const;

		[[nodiscard]]
		BenchmarkResult Run(BenchmarkRunContext& context) const;

		[[nodiscard]]
		virtual Vector<TaskConfiguration> GetRequiredConfiguration() const = 0;

		Benchmark() = default;
		Benchmark(const Benchmark&) = delete;
		Benchmark(Benchmark&&) noexcept = delete;
		Benchmark& operator=(const Benchmark&) = delete;
		Benchmark& operator=(Benchmark&&) = delete;
		virtual ~Benchmark() = default;
	 protected:

		[[nodiscard]]
		virtual Vector<UniquePtr<Task>> GetTasks(BenchmarkRunContext& context) const = 0;
		virtual void DoGetResultInfo(std::size_t coreTaskIndex, const std::vector<TaskInfo>& taskInfos, String& unit, ResultType& type) const;
		virtual void DoGetBenchmarkInfo(String& name, String& description, std::size_t& coreTaskIndex, std::vector<TaskInfo>& taskInfos) const = 0;
		virtual double CalculateResult(const std::vector<TaskInfo>& taskInfos, const std::vector<TaskResult>& taskResults, std::size_t coreTaskIndex) const;
	 private:
		static Duration ExecuteSingleThread(SharedPtr<AbstractTaskData>& data,
				UniquePtr<Task> task,
				Size& processedDataSize);
		static Duration ExecuteMultiThread(
				ConcurrencyMode concurrencyMode,
				SharedPtr<AbstractTaskData>& data,
				UniquePtr<Task> task,
				const Vector<SharedPtr<Allocator>>& allocators,
				const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
				const TopologyInfo& topologyInfo,
				Size& processedDataSize,
				bool pinThreads);

		static Duration ExecuteConcurrent(
				ConcurrencyMode concurrencyMode,
				UniquePtr<Task> task,
				SharedPtr<AbstractTaskData>& data,
				Vector<SharedPtr<AbstractTaskData>> inputData,
				const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
				const TopologyInfo& topologyInfo,
				Size& processedDataSize,
				bool pinThreads);

		void ValidateConfiguration(const Vector<TaskConfiguration>& configuration) const;

	protected:
		template<typename T>
		static UniquePtr<T> CreateTask()
		{
			auto task = std::make_unique<T>();
			return std::move(task);
		}

		template<typename T, typename ... TArgs>
		static UniquePtr<T> CreateTask(TArgs&& ... args)
		{
			auto task = std::make_unique<T>(std::forward<TArgs>(args)...);
			return std::move(task);
		}
	};

} // Elpida

#endif //ELPIDA_BENCHMARK_HPP
