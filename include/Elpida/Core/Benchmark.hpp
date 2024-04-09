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

namespace Elpida {

	class TimingInfo;
	class BenchmarkRunContext;

	class Benchmark {
	public:
		[[nodiscard]]
		virtual BenchmarkInfo GetInfo() const = 0;

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

		[[nodiscard]]
		virtual double CalculateScore(const Vector<TaskResult>& taskResults) const = 0;
	 private:
		static Duration ExecuteSingleThread(UniquePtr<AbstractTaskData>& data, UniquePtr<Task> task, const ProcessingUnitNode& topologyNode, Size& processedDataSize, bool pinThreads);
		static Duration ExecuteMultiThread(UniquePtr<AbstractTaskData>& data,
			UniquePtr<Task> task,
			const Vector<SharedPtr<Allocator>>& allocators,
			const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			Size& processedDataSize,
			bool pinThreads);

		void ValidateConfiguration(const Vector<TaskConfiguration>& configuration) const;

	protected:
		template<typename T>
		static UniquePtr<T> CreateTask(bool measured = true)
		{
			auto task = std::make_unique<T>();
			task->SetMeasured(measured);
			return std::move(task);
		}

		template<typename T, typename ... TArgs>
		static UniquePtr<T> CreateTask(bool measured, TArgs&& ... args)
		{
			auto task = std::make_unique<T>(std::forward<TArgs>(args)...);
			task->SetMeasured(measured);
			return std::move(task);
		}
	};

} // Elpida

#endif //ELPIDA_BENCHMARK_HPP
