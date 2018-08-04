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
 * TextTable.hpp
 *
 *  Created on: 11 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_TEXTTABLE_HPP_
#define ELPIDA_UTILITIES_TEXTTABLE_HPP_

#include <iostream>

#include "Elpida/Exportable.hpp"
#include "Elpida/Types/Array.hpp"
#include "Elpida/Types/String.hpp"
#include "Elpida/Types/Primitives.hpp"
#include "Elpida/Utilities/TextRow.hpp"
#include "Elpida/Utilities/TextColumn.hpp"


namespace Elpida
{

	class TextTable final: Exportable
	{
		public:

			void setDrawBorders(bool draw);
			void setPadding(Size spaces);
			void exportTo(std::ostream& output) const;

			void addRow(const TextRow& row)
			{
				_rows.push_back(row);
			}

			void addRow(TextRow&& row)
			{
				_rows.push_back(row);
			}

			template<typename ... T>
			TextTable(T ... columns)
					: _columns
						{ columns... }
			{

			}

			~TextTable()
			{

			}

			TextTable(TextTable&&) = default;
			TextTable(const TextTable&) = default;
			TextTable& operator=(TextTable&&) = default;
			TextTable& operator=(const TextTable&) = default;

		private:

			Array<TextRow> _rows;
			Array<TextColumn> _columns;
			String _newLine = "\n";
			char _cornerSymbol = '+';
			char _verticalSymbol = '|';
			char _horizontalSymbol = '-';

			void outputRowLineBorder(std::ostream& output, bool newLine = true) const;
	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_TEXTTABLE_HPP_ */
