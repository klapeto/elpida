//
// Created by klapeto on 2/3/2023.
//

#include "Elpida/Core/TaskConfiguration.hpp"

#include "Elpida/Core/ElpidaException.hpp"

namespace Elpida
{
	static const char* TranslateTypeToString(const ConfigurationType type)
	{
		switch (type)
		{
		case ConfigurationType::Integer:
			return "Integer";
		case ConfigurationType::Float:
			return "Float";
		case ConfigurationType::String:
			return "String";
		case ConfigurationType::File:
			return "File path";
		case ConfigurationType::Boolean:
			return "Boolean value (true or false)";
		}
		return "Unknown";
	}

	TaskConfiguration::TaskConfiguration(String name, ConfigurationType type, String defaultValue)
		: _name(std::move(name)), _value(std::move(defaultValue)), _type(type)
	{

	}

	void TaskConfiguration::SetValue(Integer value)
	{
		if (_type != ConfigurationType::Integer)
		{
			throw ElpidaException("Task configuration '", _name, "' required type of: ", TranslateTypeToString(_type), "but got Integer");
		}
		_value = std::to_string(value);
	}

	void TaskConfiguration::SetValue(Boolean value)
	{
		if (_type != ConfigurationType::Boolean)
		{
			throw ElpidaException("Task configuration '", _name, "' required type of: ", TranslateTypeToString(_type), "but got Integer");
		}
		_value = value ? "true" : "false";
	}

	void TaskConfiguration::SetValue(const String& value)
	{
		if (_type != ConfigurationType::String
			&& _type != ConfigurationType::File)
		{
			throw ElpidaException("Task configuration '", _name, "' required type of: ", TranslateTypeToString(_type), "but got String");
		}
		_value = value;
	}

	void TaskConfiguration::SetValue(Float value)
	{
		if (_type != ConfigurationType::Float)
		{
			throw ElpidaException("Task configuration '", _name, "' required type of: ", TranslateTypeToString(_type), "but got double");
		}
		_value = std::to_string(value);
	}

	TaskConfiguration::Integer TaskConfiguration::AsInteger() const
	{
		if (_type != ConfigurationType::Integer)
		{
			throw ElpidaException("Task configuration '", _name, " was attempted to be parsed as integer but is is: ", TranslateTypeToString(_type));
		}

		return std::stol(_value);
	}

	TaskConfiguration::Float TaskConfiguration::AsFloat() const
	{
		if (_type != ConfigurationType::Float)
		{
			throw ElpidaException("Task configuration '", _name, " was attempted to be parsed as integer but is is: ", TranslateTypeToString(_type));
		}

		return std::stod(_value);
	}

	void TaskConfiguration::Parse(const String& value)
	{
		switch (_type)
		{
		case ConfigurationType::Integer:
			SetValue(std::stol(value));
			break;
		case ConfigurationType::Float:
			SetValue(std::stod(value));
			break;
		case ConfigurationType::String:
		case ConfigurationType::File:
			SetValue(value);
			break;
		case ConfigurationType::Boolean:
			SetValue(value == "true");
			break;
		}
	}

	TaskConfiguration::Boolean TaskConfiguration::AsBoolean() const
	{
		if (_type != ConfigurationType::Boolean)
		{
			throw ElpidaException("Task configuration '", _name, " was attempted to be parsed as integer but is is: ", TranslateTypeToString(_type));
		}

		return _value == "true";
	}
} // Elpida