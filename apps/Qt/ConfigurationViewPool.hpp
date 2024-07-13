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
