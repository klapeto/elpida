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
 * SharedLibrary.hpp
 *
 *  Created on: 26 Σεπ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_SHAREDLIBRARY_HPP_
#define ELPIDA_SHAREDLIBRARY_HPP_

#include <string>

namespace Elpida
{

	class SharedLibrary final
	{
	public:

		template<typename T>
		inline T getFunctionPointer(const std::string& functionName) const
		{
			return (T)getFunctionPointerImpl(functionName);
		}

		explicit SharedLibrary(const std::string& libraryPath);
		SharedLibrary(SharedLibrary&& other) noexcept ;
		SharedLibrary& operator=(SharedLibrary&& other) noexcept ;
		~SharedLibrary();

		SharedLibrary(const SharedLibrary&) = delete;
		SharedLibrary& operator=(const SharedLibrary&) = delete;
	private:
		void* _handle;
		void* getFunctionPointerImpl(const std::string& functionName) const;
	};
} /* namespace Elpida */

#endif /* ELPIDA_SHAREDLIBRARY_HPP_ */
