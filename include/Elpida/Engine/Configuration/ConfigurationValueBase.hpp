//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP


#include <type_traits>
#include <string>
#include <utility>
#include "ConfigurationType.hpp"

namespace Elpida
{
	class ConfigurationSpecificationBase;

	/**
	 * Base class for all Configuration values
	 */
	class ConfigurationValueBase
	{
	public:

		const ConfigurationSpecificationBase& getConfigurationSpecification() const
		{
			return _configurationSpecification;
		}

		template<typename T>
		T& as()
		{
			static_assert(std::is_base_of_v<ConfigurationValueBase, T>,
				"Only derived from ConfigurationValueBase can be the template parameter");
			return static_cast<T&>(*this);
		}

		virtual ~ConfigurationValueBase() = default;
	protected:
		ConfigurationValueBase(const ConfigurationSpecificationBase& configurationSpecification)
			: _configurationSpecification(configurationSpecification)
		{
		}

		const ConfigurationSpecificationBase& _configurationSpecification;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP
