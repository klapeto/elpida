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
 * TextTable.cpp
 *
 *  Created on: 11 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/TextTable.hpp"

#include <iomanip>

#include "Elpida/TaskMetrics.hpp"

namespace Elpida
{

	void TextTable::setDrawBorders(bool draw)
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

	void TextTable::setPadding(Size spaces)
	{
		_newLine = "\n";
		for (Size i = 0; i < spaces; ++i)
		{
			_newLine += ' ';
		}
	}

	void TextTable::exportTo(std::ostream& output) const
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
				for (size_t i = 0; i < _columns.size(); ++i)
				{
					output << std::setw(_columns[i].getWidth()) << row[i] << _verticalSymbol;
				}
				output << _newLine;
			}
		}

		outputRowLineBorder(output, false);
	}

	void TextTable::outputRowLineBorder(std::ostream& output, bool newLine) const
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

} /* namespace Elpida */
