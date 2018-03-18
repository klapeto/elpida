/**************************************************************************
*   elpida - CPU benchmark tool
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

#ifndef SRC_UTILITIES_TEXTTABLE_HPP_
#define SRC_UTILITIES_TEXTTABLE_HPP_

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "Exportable.hpp"
#include "Utilities/TextColumn.hpp"
#include "Utilities/TextRow.hpp"

namespace Elpida
{

	template<int nCol>
	class TextTable: Exportable
	{
		public:

			void setDrawBorders(bool draw)
			{
				if (draw)
				{
					_cornerSymbol = '+';
					_horizontalSymbol = '-';
					_verticalSymbol = '|';
				}
				else
				{
					_cornerSymbol = ' ';
					_horizontalSymbol = ' ';
					_verticalSymbol = ' ';
				}
			}

			void setPadding(int spaces)
			{
				_newLine = "\n";
				for (int i = 0; i < spaces; ++i)
				{
					_newLine += ' ';
				}
			}

			void exportTo(std::ostream& output) const
			{

				outputRowLineBorder(output);

				output << _verticalSymbol;
				for (auto column : _columns)
				{
					output << std::setw(column.getWidth()) << column.getName() << _verticalSymbol;
				}

				outputRowLineBorder(output);

				if (_rows.size())
				{
					for (auto row : _rows)
					{
						output << std::setfill(' ') << _verticalSymbol << std::left;
						for (int i = 0; i < nCol; ++i)
						{
							output << std::setw(_columns[i].getWidth()) << row[i] << _verticalSymbol;
						}
						output << _newLine;
					}
				}

				outputRowLineBorder(output, false);
			}

			void addRow(const TextRow<nCol>& row)
			{
				_rows.push_back(row);
			}

			void addRow(TextRow<nCol> && row)
			{
				_rows.push_back(row);
			}

			template<typename ... T>
			TextTable(T ... columns) :
					_columns { columns... }
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

			std::vector<TextRow<nCol>> _rows;
			std::string _newLine = "\n";
			TextColumn _columns[nCol];
			char _cornerSymbol = '+';
			char _verticalSymbol = '|';
			char _horizontalSymbol = '-';

			void outputRowLineBorder(std::ostream& output, bool newLine = true) const
			{
				if (newLine)
				{
					output << _newLine;
				}
				output << _cornerSymbol << std::setfill(_horizontalSymbol) << std::right;
				for (auto column : _columns)
				{
					output << std::setw(column.getWidth() + 1) << _cornerSymbol;
				}
				output << std::setfill(' ') << std::left << _newLine;
			}
	};

} /* namespace Elpida */

#endif /* SRC_UTILITIES_TEXTTABLE_HPP_ */
