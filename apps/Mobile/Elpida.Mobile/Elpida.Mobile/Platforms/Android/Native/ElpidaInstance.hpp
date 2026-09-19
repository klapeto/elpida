//  Copyright (c) 2026  Ioannis Panagiotopoulos
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
// Created by klapeto on 29/6/26.
//

#ifndef ELPIDA_ELPIDAINSTANCE_HPP
#define ELPIDA_ELPIDAINSTANCE_HPP

#include "ModelBuilderJson.hpp"
#include "Elpida/Core/EnvironmentInfo.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "InProcessBenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	class FullBenchmarkInstance;
}

struct ElpidaInstance
{
	Elpida::EnvironmentInfo environmentInfo;
	Elpida::Application::ModelBuilderJson modelBuilderJson;
	Elpida::Application::TimingModel timingModel;
	Elpida::Application::TopologyModel topologyModel;
	Elpida::Application::MemoryInfoModel memoryModel;
	Elpida::InProcessBenchmarkExecutionService benchmarkExecutionService;
	Elpida::Application::BenchmarkRunConfigurationModel benchmarkRunConfigurationModel;
	std::vector<std::unique_ptr<Elpida::Application::FullBenchmarkInstance>> benchmarkInstances;
};


#endif //ELPIDA_ELPIDAINSTANCE_HPP
