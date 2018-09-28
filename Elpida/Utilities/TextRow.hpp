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
 * TextRow.hpp
 *
 *  Created on: 11 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_TEXTROW_HPP_
#define ELPIDA_UTILITIES_TEXTROW_HPP_

#include "Elpida/Types/Array.hpp"
#include "Elpida/Types/String.hpp"
#include "Elpida/Types/Primitives.hpp"
#include "Elpida/Exceptions/OutOfRangeException.hpp"

namespace Elpida
{

	class TextRow final
	{
		public:

			const String& operator [](Index i) const
			{
				if (i >= 0 && i < _data[0].size()) return _data[i];
				throw OutOfRangeException("TextRow", "TextRow[] called with i out of range!");
			}

			template<typename ... T>
			TextRow(T ... data)
					: _data
						{ data... }
			{
			}

			~TextRow()
			{

			}

			TextRow(TextRow&&) = default;
			TextRow(const TextRow&) = default;
			TextRow& operator=(TextRow&&) = default;
			TextRow& operator=(const TextRow&) = default;
		private:
			Array<String> _data;

	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_TEXTROW_HPP_ */