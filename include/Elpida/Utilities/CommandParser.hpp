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
 *   GNU General PuELPIDA_UTILITIESblic License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * CommandParser.hpp
 *
 *  Created on: 28 Ιουλ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_COMMANDPARSER_HPP_
#define ELPIDA_UTILITIES_COMMANDPARSER_HPP_

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace Elpida
{

	class CommandParser
	{
	public:
		bool argumentExists(const std::string& name) const;
		const std::string& getArgument(const std::string& name) const;
		const std::string& getArgument(std::size_t index) const;

		const std::unordered_map<std::string, std::string>& getNamedArguments() const
		{
			return _namedArguments;
		}

		const std::vector<std::string>& getUnamedArguments() const
		{
			return _unamedArguments;
		}

		void parseCommand(const std::string& input);

		CommandParser()
			: _notFound("")
		{
		}

		~CommandParser()
		{
		}
	private:
		std::vector<std::string> _unamedArguments;
		std::unordered_map<std::string, std::string> _namedArguments;
		std::string _notFound;
	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_COMMANDPARSER_HPP_ */
