//
// Created by klapeto on 9/4/2024.
//

#ifndef ELPIDA_BENCHMARKRUNCONTEXT_HPP
#define ELPIDA_BENCHMARKRUNCONTEXT_HPP

#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Core/TaskConfiguration.hpp"
#include "Elpida/Core/AllocatorFactory.hpp"
#include "Elpida/Core/ConcurrencyMode.hpp"

namespace Elpida
{
	class EnvironmentInfo;
	class BenchmarkRunContext
	{
	public:
		[[nodiscard]]
		const Vector<Ref<const ProcessingUnitNode>>& GetTargetProcessors() const;

		[[nodiscard]]
		const Vector<TaskConfiguration>& GetConfiguration() const;

		[[nodiscard]]
		const EnvironmentInfo& GetEnvironmentInfo() const;

		[[nodiscard]]
		bool IsPinThreads() const;

		[[nodiscard]]
		const UniquePtr<AllocatorFactory>& GetAllocatorFactory() const;

		[[nodiscard]]
		ConcurrencyMode GetConcurrencyMode() const;

		explicit BenchmarkRunContext(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
				const Vector<TaskConfiguration>& configuration,
				UniquePtr<AllocatorFactory> allocatorFactory, const EnvironmentInfo& environmentInfo, ConcurrencyMode concurrencyMode, bool pinThreads);
	private:
		const Vector<Ref<const ProcessingUnitNode>>& _targetProcessors;
		const Vector<TaskConfiguration>& _configuration;
		const EnvironmentInfo& _environmentInfo;
		UniquePtr<AllocatorFactory> _allocatorFactory;
		ConcurrencyMode _concurrencyMode;
		bool _pinThreads;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKRUNCONTEXT_HPP
