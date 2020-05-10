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
}