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

#ifndef ELPIDA_CONFIGURATIONVIEWPOOL_HPP_
#define ELPIDA_CONFIGURATIONVIEWPOOL_HPP_

#include <vector>
#include <unordered_map>
#include <memory>

#include "Controllers/BenchmarkConfigurationInstanceController.hpp"
#include "Views/ConfigurationViews/ConfigurationView.hpp"

namespace Elpida::Application
{

	class BenchmarkConfigurationModel;
	class ConfigurationView;
	class SettingsService;

	class ConfigurationViewPool final
	{
	 public:
		ConfigurationView* RentViewForModel(const BenchmarkConfigurationModel& configurationModel);
		void ReturnViewFromModel(const BenchmarkConfigurationModel& configurationModel, ConfigurationView* view);

		explicit ConfigurationViewPool(SettingsService& settingsService);
		ConfigurationViewPool(const ConfigurationViewPool&) = delete;
		ConfigurationViewPool(ConfigurationViewPool&&) noexcept = delete;
		ConfigurationViewPool& operator=(const ConfigurationViewPool&) = delete;
		ConfigurationViewPool& operator=(ConfigurationViewPool&&) noexcept = delete;
		~ConfigurationViewPool() = default;
	 private:
		struct CreatedInstance
		{
			ConfigurationView* view;
			BenchmarkConfigurationInstanceController controller;
		};

		std::vector<std::unique_ptr<ConfigurationView>> _fileViews;
		std::vector<std::unique_ptr<ConfigurationView>> _floatViews;
		std::vector<std::unique_ptr<ConfigurationView>> _integerViews;
		std::vector<std::unique_ptr<ConfigurationView>> _stringViews;
		std::vector<std::unique_ptr<ConfigurationView>> _booleanViews;
		std::unordered_map<const BenchmarkConfigurationModel*, CreatedInstance> _rentedInstances;
		SettingsService& _settingsService;

		template<typename T>
		std::unique_ptr<ConfigurationView> GetOrCreate(std::vector<std::unique_ptr<ConfigurationView>>& pool)
		{
			if (!pool.empty())
			{
				auto view = std::move(pool.back());
				pool.pop_back();
				return view;
			}
			else
			{
				return std::make_unique<T>();
			}
		}
	};

} // Application

#endif //ELPIDA_CONFIGURATIONVIEWPOOL_HPP_
