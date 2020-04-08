/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * FileSystem.hpp
 *
 *  Created on: 27 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_FILESYSTEM_HPP_
#define ELPIDA_UTILITIES_FILESYSTEM_HPP_

#include <functional>
#include <sstream>
#include <string>

#include "Elpida/Config.hpp"

namespace Elpida
{

	class FileSystem
	{
	public:
#ifdef ELPIDA_LINUX
		static constexpr char _pathSeparator = '/';
#else
		static constexpr char _pathSeparator = '\\';
#endif

		static void iterateDirectory(const std::string& directory, std::function<void(const std::string&)> func);
		static bool fileExists(const std::string& file);

		template<typename ... T>
		static std::string concatPaths(T ... args)
		{
			std::ostringstream buffer;
			concatPathsImlFirst(buffer, args...);
			return buffer.str();
		}
	private:
		template<typename T, typename ... Rest>
		static void concatPathsImlFirst(std::ostringstream& buffer, T arg, Rest ... rest)
		{
			buffer << arg;
			concatPathsIml(buffer, rest...);
		}

		template<typename T, typename ... Rest>
		static void concatPathsIml(std::ostringstream& buffer, T arg, Rest ... rest)
		{
			buffer << _pathSeparator << arg;
			concatPathsIml(buffer, rest...);
		}

		template<typename T>
		static void concatPathsIml(std::ostringstream& buffer, T arg)
		{
			buffer << _pathSeparator << arg;
		}

		static inline void concatPathsIml(std::ostringstream& buffer)
		{

		}
	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_FILESYSTEM_HPP_ */
