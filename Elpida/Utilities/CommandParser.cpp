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
 * CommandParser.cpp
 *
 *  Created on: 28 Ιουλ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/CommandParser.hpp"

#include "Elpida/Exceptions/ParserException.hpp"

namespace Elpida
{
	static inline bool isWhiteSpace(char c)
	{
		return c == ' ' || c == '\t' || c == '\n';
	}

	const String& CommandParser::getArgument(const String& name) const
	{
		auto itr = _namedArguments.find(name);
		return itr != _namedArguments.end() ? itr->second : _notFound;
	}

	bool CommandParser::argumentExists(const String& name) const
	{
		return _namedArguments.find(name) != _namedArguments.end() ? true : false;
	}

	const String& CommandParser::getArgument(Index index) const
	{
		return index < _unamedArguments.size() ? _unamedArguments[index] : _notFound;
	}

	void Elpida::CommandParser::parseCommand(const String& input)
	{
		_unamedArguments.clear();
		_namedArguments.clear();
		const char* front = input.c_str();
		const char* back = front;
		while (*front)
		{
			while (isWhiteSpace(*front) && *front)
			{
				back++;
				front++;
			}
			while (!isWhiteSpace(*front) && *front)
				front++;

			if (*back == '-')
			{
				const char* argumentNameStart = ++back;
				const char* argumentNameEnd = back;

				while (*argumentNameEnd != '=' && !isWhiteSpace(*argumentNameEnd) && *argumentNameEnd)
					argumentNameEnd++;

				String argumentName = String(argumentNameStart, argumentNameEnd);
				switch (*argumentNameEnd)
				{
					case '=':
						argumentNameStart = ++argumentNameEnd;
						while (!isWhiteSpace(*argumentNameEnd) && *argumentNameEnd)
							argumentNameEnd++;
						_namedArguments.emplace(argumentName, String(argumentNameStart, argumentNameEnd));
						break;
					case 0:
					case ' ':
						_namedArguments.emplace(argumentName, "true");
						break;
				}
			}
			else
			{
				if (back != front)
				{
					_unamedArguments.push_back(String(back, front));
				}
			}

			back = front;
		}
	}

} /* namespace Elpida */

