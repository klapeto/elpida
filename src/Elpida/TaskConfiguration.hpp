//
// Created by klapeto on 2/3/2023.
//

#ifndef ELPIDA_TASKCONFIGURATION_HPP_
#define ELPIDA_TASKCONFIGURATION_HPP_

#include "Elpida/String.hpp"

namespace Elpida
{

	enum class ConfigurationType
	{
		Integer,
		Float,
		String,
		File,
	};

	class TaskConfiguration final
	{
	 public:
		using File = std::string;
		using Integer = long;
		using Float = double;
		using String = std::string;

		[[nodiscard]]
		const String& GetName() const
		{
			return _name;
		}

		[[nodiscard]]
		const String& GetValue() const
		{
			return _value;
		}

		[[nodiscard]]
		ConfigurationType GetType() const
		{
			return _type;
		}

		void SetValue(Integer value);
		void SetValue(const String& value);
		void SetValue(Float value);

		Integer AsInteger();
		Float AsFloat();

		TaskConfiguration(String name, ConfigurationType type);
		TaskConfiguration(const TaskConfiguration&) = default;
		TaskConfiguration(TaskConfiguration&&) noexcept = default;
		TaskConfiguration& operator=(const TaskConfiguration&) = default;
		TaskConfiguration& operator=(TaskConfiguration&&) noexcept = default;
		~TaskConfiguration() = default;
	 private:
		String _name;
		String _value;
		ConfigurationType _type;
	};

} // Elpida

#endif //ELPIDA_TASKCONFIGURATION_HPP_
