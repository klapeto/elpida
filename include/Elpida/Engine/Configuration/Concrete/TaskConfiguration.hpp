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

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATION_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATION_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include <optional>
#include "ConfigurationValueBase.hpp"

namespace Elpida
{
	class TaskSpecification;
	class TaskBuilder;

	/**
	 * Configuration values for a specific Task instance
	 */
	class TaskConfiguration final
	{
	public:
		std::optional<std::shared_ptr<ConfigurationValueBase>> getConfiguration(const std::string& name) const;
		void setConfiguration(const std::string& name, std::shared_ptr<ConfigurationValueBase> configuration);

		const std::unordered_map<std::string, std::shared_ptr<ConfigurationValueBase>>& getAllConfigurations() const
		{
			return _configuration;
		}

		bool isEnabled() const
		{
			return _enabled;
		}

		void setEnabled(bool enabled)
		{
			_enabled = enabled;
		}

		const TaskSpecification& getTaskSpecification() const
		{
			return *_taskSpecification;
		}

		const TaskBuilder& getTaskBuilder() const
		{
			return *_taskBuilder;
		}

		explicit TaskConfiguration(const TaskSpecification& taskSpecification, const TaskBuilder& taskBuilder)
			: _taskSpecification(&taskSpecification), _taskBuilder(&taskBuilder), _enabled(true)
		{

		}

		TaskConfiguration(const TaskConfiguration& other);
		TaskConfiguration& operator=(const TaskConfiguration& other);
		TaskConfiguration(TaskConfiguration&&) noexcept;
		~TaskConfiguration() = default;
	private:
		std::unordered_map<std::string, std::shared_ptr<ConfigurationValueBase>> _configuration;
		const TaskSpecification* _taskSpecification;
		const TaskBuilder* _taskBuilder;
		bool _enabled;

		void copy(const TaskConfiguration& other);
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATION_HPP
