//
// Created by klapeto on 2/3/2023.
//

#include "Elpida/TaskConfiguration.hpp"
#include "Elpida/ElpidaException.hpp"

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
		}
		return "Unknown";
	}

	TaskConfiguration::TaskConfiguration(String name, ConfigurationType type, String defaultValue)
		: _name(std::move(name)), _type(type), _value(std::move(defaultValue))
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

	TaskConfiguration::Integer TaskConfiguration::AsInteger()
	{
		if (_type != ConfigurationType::Integer)
		{
			throw ElpidaException("Task configuration '", _name, " was attempted to be parsed as integer but is is: ", TranslateTypeToString(_type));
		}

		return std::stol(_value);
	}

	TaskConfiguration::Float TaskConfiguration::AsFloat()
	{
		if (_type != ConfigurationType::Float)
		{
			throw ElpidaException("Task configuration '", _name, " was attempted to be parsed as integer but is is: ", TranslateTypeToString(_type));
		}

		return std::stod(_value);
	}
} // Elpida