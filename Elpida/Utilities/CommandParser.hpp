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
 * CommandParser.hpp
 *
 *  Created on: 28 Ιουλ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_COMMANDPARSER_HPP_
#define ELPIDA_UTILITIES_COMMANDPARSER_HPP_

#include "Elpida/Types/Map.hpp"
#include "Elpida/Types/Array.hpp"
#include "Elpida/Types/String.hpp"
#include "Elpida/Types/Primitives.hpp"

namespace Elpida
{

	class CommandParser
	{
		public:
			bool argumentExists(const String& name) const;
			const String& getArgument(const String& name) const;
			const String& getArgument(Index index) const;

			const Map<String, String>& getNamedArguments() const
			{
				return _namedArguments;
			}

			const Array<String>& getUnamedArguments() const
			{
				return _unamedArguments;
			}

			void parseCommand(const String& input);

			CommandParser()
					: _notFound("")
			{
			}

			~CommandParser()
			{
			}
		private:
			Array<String> _unamedArguments;
			Map<String, String> _namedArguments;
			String _notFound;
	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_COMMANDPARSER_HPP_ */
