//
// Created by klapeto on 22/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONTYPE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONTYPE_HPP

#include <string>

namespace Elpida
{
	class ConfigurationType
	{
	public:

		using Int = long;
		using UnsignedInt = unsigned long;
		using Float = double;
		using String = std::string;
		using Bool = bool;
		using FilePath = std::string;
		using FolderPath = std::string;
		using Custom = void*;

		enum class Type
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

		ConfigurationType() = delete;
		~ConfigurationType() = delete;
	};

}

#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONTYPE_HPP
