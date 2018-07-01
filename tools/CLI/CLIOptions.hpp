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
 * Options.hpp
 *
 *  Created on: 26 Ιουν 2018
 *      Author: klapeto
 */

#ifndef SRC_TOOLS_CLI_CLIOPTIONS_HPP_
#define SRC_TOOLS_CLI_CLIOPTIONS_HPP_

#include "Utilities/Singleton.hpp"

namespace Elpida
{
	namespace CLI
	{

		class CLIOptions final: public Singleton<CLIOptions>
		{
			public:
				bool isShowCpuId() const
				{
					return _showCpuId;
				}

				bool isVerbose() const
				{
					return _verbose;
				}

				void parseArguments(int argC, char** argV);

			private:
				bool _showCpuId;
				bool _showHelp;
				bool _verbose;

				CLIOptions()
						: _showCpuId(false), _showHelp(false), _verbose(false)
				{
				}

				virtual ~CLIOptions()
				{
				}
		};

	} /* namespace CLI */
} /* namespace Elpida */

#endif /* SRC_TOOLS_CLI_CLIOPTIONS_HPP_ */
