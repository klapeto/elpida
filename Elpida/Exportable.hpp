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
 * IExportable.hpp
 *
 *  Created on: 10 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_EXPORTABLE_HPP_
#define ELPIDA_EXPORTABLE_HPP_

#include <ostream>

#include "Elpida/Types/String.hpp"
#include "Elpida/Types/Primitives.hpp"

namespace Elpida
{

	class Exportable
	{
		public:
			virtual void setPadding(Size spaces)
			{
				_newLine = "\n";
				for (Size i = 0; i < spaces; ++i)
				{
					_newLine += ' ';
				}
			}

			virtual void exportTo(std::ostream& output) const = 0;

			Exportable()
			{
			}

			virtual ~Exportable()
			{
			}
		protected:
			String _newLine = "\n";
	};

} /* namespace Elpida */

#endif /* ELPIDA_EXPORTABLE_HPP_ */
