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

#include "Elpida/Types/String.hpp"

namespace Elpida
{

	class SharedLibrary final
	{
		public:

			template<typename T>
			inline T getFunctionPointer(const String& functionName) const
			{
				return (T) getFunctionPointerImpl(functionName);
			}

			SharedLibrary(const String& libraryPath);
			SharedLibrary(const SharedLibrary&) = delete;
			SharedLibrary& operator=(const SharedLibrary&) = delete;

			SharedLibrary(SharedLibrary&& other)
			{
				this->_handle = other._handle;
				other._handle = nullptr;
			}
			SharedLibrary& operator=(SharedLibrary&& other)
			{
				this->_handle = other._handle;
				other._handle = nullptr;
				return *this;
			}

			~SharedLibrary();
		private:
			void* _handle;
			void* getFunctionPointerImpl(const String& functionName) const;
	};
} /* namespace Elpida */

#endif /* ELPIDA_SHAREDLIBRARY_HPP_ */
