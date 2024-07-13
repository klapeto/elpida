//
// Created by klapeto on 9/4/2024.
//

#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{
	BenchmarkRunContext::BenchmarkRunContext(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			const Vector<TaskConfiguration>& configuration,
			UniquePtr<AllocatorFactory> allocatorFactory, const EnvironmentInfo& environmentInfo, ConcurrencyMode concurrencyMode, bool pinThreads)
			: _targetProcessors(targetProcessors), _configuration(configuration),
			  _environmentInfo(environmentInfo), _allocatorFactory(std::move(allocatorFactory)), _concurrencyMode(concurrencyMode), _pinThreads(pinThreads)
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

	const EnvironmentInfo& BenchmarkRunContext::GetEnvironmentInfo() const
	{
		return _environmentInfo;
	}

	const UniquePtr<AllocatorFactory>& BenchmarkRunContext::GetAllocatorFactory() const
	{
		return _allocatorFactory;
	}

	bool BenchmarkRunContext::IsPinThreads() const
	{
		return _pinThreads;
	}

	ConcurrencyMode BenchmarkRunContext::GetConcurrencyMode() const
	{
		return _concurrencyMode;
	}
} // Elpida