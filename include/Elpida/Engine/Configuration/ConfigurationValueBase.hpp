//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP


#include <type_traits>
#include <string>
#include <utility>

namespace Elpida
{
	/**
	 * Base class for all Configuration values
	 */
	class ConfigurationValueBase
	{
	public:
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

		[[nodiscard]] Type getType() const
		{
			return _type;
		}

		template<typename T>
		T& as()
		{
			static_assert(std::is_base_of_v<ConfigurationValueBase, T>,
				"Only derived from ConfigurationValueBase can be the template parameter");
			return static_cast<T&>(*this);
		}

		explicit ConfigurationValueBase(Type type, std::string name, std::string description)
			: _type(type), _name(std::move(name)), _description(std::move(description))
		{
		}

		virtual ~ConfigurationValueBase() = default;
	protected:
		std::string _name;
		std::string _description;
		Type _type;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_CONFIGURATIONVALUEBASE_HPP
