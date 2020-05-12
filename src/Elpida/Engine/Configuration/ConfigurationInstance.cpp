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

	ConfigurationInstance::ConfigurationInstance(ConfigurationInstance&& other)
	{
		move(std::move(other));
	}

	ConfigurationInstance& ConfigurationInstance::operator=(ConfigurationInstance&& other)
	{
		move(std::move(other));
		return *this;
	}

	void ConfigurationInstance::move(ConfigurationInstance&& other)
	{
		_configurationSpecification = other._configurationSpecification;
		other._configurationSpecification = nullptr;
		_value = other._value;
		other._value = nullptr;
		_fixed = other._fixed;
	}
}