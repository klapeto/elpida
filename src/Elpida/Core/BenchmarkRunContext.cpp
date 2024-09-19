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

#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{
	BenchmarkRunContext::BenchmarkRunContext(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			const Vector<TaskConfiguration>& configuration, UniquePtr<AllocatorFactory> allocatorFactory,
			const EnvironmentInfo& environmentInfo, ConcurrencyMode concurrencyMode, bool pinThreads,
			Duration minimumMicroTaskDuration)
			:_targetProcessors(targetProcessors), _configuration(configuration),
			 _environmentInfo(environmentInfo), _allocatorFactory(std::move(allocatorFactory)), _concurrencyMode(concurrencyMode), _pinThreads(pinThreads),
			 _minimumMicroTaskDuration(minimumMicroTaskDuration)
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

	const Duration& BenchmarkRunContext::GetMinimumMicroTaskDuration() const
	{
		return _minimumMicroTaskDuration;
	}
} // Elpida