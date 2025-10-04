/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_RESULTTYPE_HPP
#define ELPIDA_RESULTTYPE_HPP

namespace Elpida
{

	/**
	 * @brief Defines what kind a score is.
	 */
	enum class ResultType
	{
		/**
		 * @brief The score it measured in units/s.
		 */
		Throughput,

		/**
		 * @brief The score is execution time.
		 */
		Time,

		/**
		 * @brief The score is implementation defined.
		 */
		Custom
	};
}
#endif //ELPIDA_RESULTTYPE_HPP
