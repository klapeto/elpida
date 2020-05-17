//
// Created by klapeto on 10/5/20.
//

#include "Elpida/Engine/Configuration/ConfigurationInstance.hpp"
#include "Elpida/Engine/Configuration/Concrete/ConfigurationValueBase.hpp"

namespace Elpida
{

	ConfigurationInstance::~ConfigurationInstance()
	{
		delete _value;
	}

	ConfigurationInstance::ConfigurationInstance(ConfigurationInstance&& other) noexcept
	{
		move(std::move(other));
	}

	ConfigurationInstance& ConfigurationInstance::operator=(ConfigurationInstance&& other) noexcept
	{
		move(std::move(other));
		return *this;
	}

	void ConfigurationInstance::move(ConfigurationInstance&& other) noexcept
	{
		_configurationSpecification = other._configurationSpecification;
		other._configurationSpecification = nullptr;
		_value = other._value;
		other._value = nullptr;
		_fixed = other._fixed;
	}
}