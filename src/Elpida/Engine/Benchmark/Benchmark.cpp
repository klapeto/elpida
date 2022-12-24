/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Benchmark/Benchmark.hpp"

#include <utility>
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Configuration/Concrete/BenchmarkConfiguration.hpp"
#include "Elpida/Engine/Task/MultiThreadTask.hpp"
#include "Elpida/Utilities/Uuid.hpp"
#include "Elpida/Engine/Calculators/BenchmarkScoreCalculator.hpp"

namespace Elpida
{
	Benchmark::Benchmark(std::string name,
			const BenchmarkScoreSpecification& scoreSpecification,
			std::shared_ptr<BenchmarkScoreCalculator> scoreCalculator,
			std::string uuid)
			:
			_scoreSpecification(scoreSpecification),
			_name(std::move(name)),
			_scoreCalculator(std::move(scoreCalculator))
	{
		_id = !uuid.empty() ? std::move(uuid) : _name + Uuid::create();
	}

	std::vector<BenchmarkTaskInstance> Benchmark::createNewTasks(const TaskAffinity& affinity,
			const BenchmarkConfiguration& configuration,
			const ServiceProvider& serviceProvider) const
	{
		if (!affinity.getProcessorNodes().empty())
		{
			std::vector<BenchmarkTaskInstance> tasks;
			try
			{
				for (const auto& builder : _taskBuilders)
				{
					auto taskConfiguration = configuration.getConfigurationForTask(builder);
					if (!taskConfiguration.has_value()
						&& !builder.getDefaultConfiguration().getAllConfigurations().empty())
					{
						throw ElpidaException(FUNCTION_NAME,
								Vu::Cs("Task: '",
										builder.getTaskSpecification().getName(),
										"' requires configuration that was not provided!"));
					}
					if (builder.shouldBeExecuted(*taskConfiguration, *affinity.getProcessorNodes().front(),
							serviceProvider)
						&& (!builder.canBeDisabled() || taskConfiguration->get().isEnabled()))
					{
						if (builder.canBeMultiThreaded())
						{
							tasks.emplace_back(std::make_unique<MultiThreadTask>(builder,
									*taskConfiguration,
									affinity,
									serviceProvider,
									builder.getIterationsToRun()), builder);
						}
						else
						{
							tasks.emplace_back(
									builder.build(
											*taskConfiguration,
											*affinity.getProcessorNodes().front(),
											serviceProvider),
									builder);
						}
					}
				}
			}
			catch (...)
			{
				throw;
			}
			return tasks;
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME, "Benchmark affinity is empty");
		}
	}
}