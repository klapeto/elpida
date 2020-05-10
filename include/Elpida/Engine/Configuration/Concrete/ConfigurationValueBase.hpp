//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP


#include <type_traits>
#include <string>
#include <utility>
#include "Elpida/Engine/Configuration/ConfigurationType.hpp"

namespace Elpida
{
	class ConfigurationSpecificationBase;

	/**
	 * Base class for all Configuration values
	 */
	class ConfigurationValueBase
	{
	public:

		[[nodiscard]] const ConfigurationSpecificationBase& getConfigurationSpecification() const
		{
			return _configurationSpecification;
		}

		[[nodiscard]] bool isReadOnly() const
		{
			return _readOnly;
		}

		[[nodiscard]] virtual ConfigurationValueBase* clone() const = 0;

		template<typename T>
		T& as()
		{
			static_assert(std::is_base_of_v<ConfigurationValueBase, T>,
				"Only derived from ConfigurationValueBase can be the template parameter");
			return static_cast<T&>(*this);
		}

		virtual ~ConfigurationValueBase() = default;
	protected:
		explicit ConfigurationValueBase(const ConfigurationSpecificationBase& configurationSpecification, bool readOnly)
			: _configurationSpecification(configurationSpecification), _readOnly(readOnly)
		{
		}

		const ConfigurationSpecificationBase& _configurationSpecification;
	private:
		bool _readOnly;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP
