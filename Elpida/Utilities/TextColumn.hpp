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
 * TextColumn.hpp
 *
 *  Created on: 11 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_TEXTCOLUMN_HPP_
#define ELPIDA_UTILITIES_TEXTCOLUMN_HPP_

#include "Elpida/Types/String.hpp"
#include "Elpida/Types/Primitives.hpp"

namespace Elpida
{

	class TextColumn final
	{
		public:
			const String& getName() const
			{
				return _name;
			}

			void setName(const String& name)
			{
				_name = name;
			}

			Size getWidth() const
			{
				return _width;
			}

			void setWidth(Size width)
			{
				_width = width;
			}

			TextColumn(const String& name, Size width)
					: _name(name), _width(width)
			{

			}

			~TextColumn()
			{

			}

			TextColumn(TextColumn&&) = default;
			TextColumn(const TextColumn&) = default;
			TextColumn& operator=(TextColumn&&) = default;
			TextColumn& operator=(const TextColumn&) = default;
		private:
			String _name;
			Size _width;
	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_TEXTCOLUMN_HPP_ */
