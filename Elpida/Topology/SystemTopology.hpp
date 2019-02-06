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
 * SystemTopology.hpp
 *
 *  Created on: 12 Ιαν 2019
 *      Author: klapeto
 */

#ifndef ELPIDA_TOPOLOGY_SYSTEMTOPOLOGY_HPP_
#define ELPIDA_TOPOLOGY_SYSTEMTOPOLOGY_HPP_

namespace Elpida
{

	class ProcessorNode;

	class SystemTopology final
	{
		public:
			unsigned int getDepth() const
			{
				return _depth;
			}

			const ProcessorNode* getRoot() const
			{
				return _root;
			}

			SystemTopology();
			~SystemTopology();
		private:
			ProcessorNode* _root;
			unsigned int _depth;
			void reload();
	};

} /* namespace Elpida */

#endif /* ELPIDA_TOPOLOGY_SYSTEMTOPOLOGY_HPP_ */
