/*
 * FileSystem.hpp
 *
 *  Created on: 27 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_FILESYSTEM_HPP_
#define ELPIDA_UTILITIES_FILESYSTEM_HPP_

#include <functional>
#include "Elpida/Types/String.hpp"
#include "Elpida/Config.hpp"

#if _elpida_linux
#define PATH_SEPERATOR '/'
#elif _elpida_windows
#define PATH_SEPERATOR '\\'
#endif

namespace Elpida
{

	class FileSystem
	{
		public:
			static void iterateDirectory(const String& directory, std::function<void(const String&)> func);
			static bool fileExists(const String& file);
	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_FILESYSTEM_HPP_ */
