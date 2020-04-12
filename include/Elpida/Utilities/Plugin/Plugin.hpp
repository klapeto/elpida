//
// Created by klapeto on 12/4/20.
//

#ifndef INCLUDE_ELPIDA_UTILITIES_PLUGIN_HPP
#define INCLUDE_ELPIDA_UTILITIES_PLUGIN_HPP


#include <algorithm>

namespace Elpida
{
	template<typename T>
	class Plugin
	{
	public:

		virtual const T& getUnderlyingData() const = 0;
		virtual ~Plugin() = default;
	};
}


#endif //INCLUDE_ELPIDA_UTILITIES_PLUGIN_HPP
