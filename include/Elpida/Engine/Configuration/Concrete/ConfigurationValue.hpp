//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUE_HPP


#include "ConfigurationValueBase.hpp"
#include <string>

namespace Elpida
{
	/**
	 * Type-Specific Configuration Value
	 * @tparam T Underlying type that will be used. The type must be compatible with the Type
	 */
	template<typename T>
	class ConfigurationValue : public ConfigurationValueBase
	{
	public:

		T getValue() const
		{
			return _value;
		}

		[[nodiscard]] ConfigurationValueBase* clone() const override
		{
			return new ConfigurationValue<T>(_configurationSpecification, (const T&)_value, isReadOnly());
		}

		void setValue(T value)
		{
			_value = value;
		}

		explicit ConfigurationValue(const ConfigurationSpecificationBase& specification, T value, bool readOnly = false)
			: ConfigurationValueBase(specification, readOnly), _value(value)
		{

		}
		~ConfigurationValue() override = default;
	private:
		T _value;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUE_HPP
