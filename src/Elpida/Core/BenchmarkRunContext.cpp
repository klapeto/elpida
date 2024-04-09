//
// Created by klapeto on 9/4/2024.
//

#include "BenchmarkRunContext.hpp"

namespace Elpida
{
	BenchmarkRunContext::BenchmarkRunContext(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			const Vector<TaskConfiguration>& configuration,
			ThreadPool& threadPool,
			UniquePtr<AllocatorFactory> allocatorFactory,
			const EnvironmentInfo& environmentInfo)
			: _targetProcessors(targetProcessors), _configuration(configuration), _threadPool(threadPool),
			  _environmentInfo(environmentInfo), _allocatorFactory(std::move(allocatorFactory))
	{
	}

	const Vector<Ref<const ProcessingUnitNode>>& BenchmarkRunContext::GetTargetProcessors() const
	{
		return _targetProcessors;
	}

	const Vector<TaskConfiguration>& BenchmarkRunContext::GetConfiguration() const
	{
		return _configuration;
	}

	ThreadPool& BenchmarkRunContext::GetThreadPool() const
	{
		return _threadPool;
	}

	const EnvironmentInfo& BenchmarkRunContext::GetEnvironmentInfo() const
	{
		return _environmentInfo;
	}

	const UniquePtr<AllocatorFactory>& BenchmarkRunContext::GetAllocatorFactory() const
	{
		return _allocatorFactory;
	}
} // Elpida