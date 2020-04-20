//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUE_HPP


#include "ConfigurationValueBase.hpp"
#include <string>

namespace Elpida
{
	template<typename T>
	class ConfigurationValue : public ConfigurationValueBase
	{
	public:
		T getValue() const
		{
			return _value;
		}

		void setValue(T value)
		{
			_value = value;
		}

		explicit ConfigurationValue(bool value, const std::string& description = std::string())
			: ConfigurationValue(Type::Bool, value)
		{
			static_assert(std::is_same_v<bool, T>, "ConfigurationValue is not specified as bool");
		}

		explicit ConfigurationValue(unsigned long value, const std::string& description = std::string())
			: ConfigurationValue(Type::UnsignedInt, value)
		{
			static_assert(std::is_literal_type_v<T>, "ConfigurationValue is not specified as unsigned integral");
		}

		explicit ConfigurationValue(long value, const std::string& description = std::string())
			: ConfigurationValue(Type::Int, value)
		{
			static_assert(std::is_literal_type_v<T>, "ConfigurationValue is not specified as integral");
		}

		explicit ConfigurationValue(double value, const std::string& description = std::string())
			: ConfigurationValue(Type::Float, value)
		{
			static_assert(std::is_floating_point_v<T>, "ConfigurationValue is not specified as floating point");
		}

		explicit ConfigurationValue(const std::string& value, const std::string& description = std::string())
			: ConfigurationValue(Type::String, value)
		{
			static_assert(std::is_convertible_v<std::string, T>, "ConfigurationValue is not specified as string");
		}

		ConfigurationValue(Type type, T value, const std::string& description = std::string())
			: ConfigurationValueBase(type, description), _value(value)
		{
		}

		~ConfigurationValue() override = default;
	private:
		T _value;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUE_HPP
