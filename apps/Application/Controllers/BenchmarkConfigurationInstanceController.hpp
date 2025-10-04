/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 17/4/2023.
//

#ifndef ELPIDA_BENCHMARKCONFIGURATIONINSTANCECONTROLLER_HPP_
#define ELPIDA_BENCHMARKCONFIGURATIONINSTANCECONTROLLER_HPP_

#include "Controller.hpp"
#include "Models/Benchmark/BenchmarkConfigurationModel.hpp"

namespace Elpida::Application
{

	class SettingsService;

	class BenchmarkConfigurationInstanceController : public Controller<BenchmarkConfigurationModel>
	{
	 public:
		void SetValue(std::string value);
		explicit BenchmarkConfigurationInstanceController(BenchmarkConfigurationModel& model, SettingsService& settingsService);
	private:
		SettingsService& _settingsService;
	};

} // Application

#endif //ELPIDA_BENCHMARKCONFIGURATIONINSTANCECONTROLLER_HPP_
