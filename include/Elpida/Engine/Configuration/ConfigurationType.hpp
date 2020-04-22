//
// Created by klapeto on 22/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONTYPE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONTYPE_HPP

namespace Elpida
{
	enum class ConfigurationType
	{
		String,
		Bool,
		Int,
		UnsignedInt,
		Float,
		FilePath,
		FolderPath,
		Custom,
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONTYPE_HPP
