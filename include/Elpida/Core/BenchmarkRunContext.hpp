//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

		[[nodiscard]]
		const Duration& GetMinimumMicroTaskDuration() const;

		explicit BenchmarkRunContext(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
				const Vector<TaskConfiguration>& configuration, UniquePtr<AllocatorFactory> allocatorFactory,
				const EnvironmentInfo& environmentInfo, ConcurrencyMode concurrencyMode, bool pinThreads,
				Duration minimumMicroTaskDuration);
	private:
		const Vector<Ref<const ProcessingUnitNode>>& _targetProcessors;
		const Vector<TaskConfiguration>& _configuration;
		const EnvironmentInfo& _environmentInfo;
		UniquePtr<AllocatorFactory> _allocatorFactory;
		Duration _minimumMicroTaskDuration;
		ConcurrencyMode _concurrencyMode;
		bool _pinThreads;
	};

} // Elpida

#endif //ELPIDA_BENCHMARKRUNCONTEXT_HPP
