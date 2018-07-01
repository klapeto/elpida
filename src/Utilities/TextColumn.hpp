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

#ifndef SRC_UTILITIES_TEXTCOLUMN_HPP_
#define SRC_UTILITIES_TEXTCOLUMN_HPP_

#include <string>

namespace Elpida
{

	class TextColumn
	{
		public:
			const std::string& getName() const
			{
				return _name;
			}

			void setName(const std::string& name)
			{
				_name = name;
			}

			int getWidth() const
			{
				return _width;
			}

			void setWidth(int width)
			{
				_width = width;
			}

			TextColumn(const std::string& name, int width) :
					_name(name), _width(width)
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
			std::string _name;
			int _width;
	};

} /* namespace Elpida */

#endif /* SRC_UTILITIES_TEXTCOLUMN_HPP_ */
