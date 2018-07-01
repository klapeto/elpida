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
 * CpuFeature.hpp
 *
 *  Created on: 11 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_CPUFEATURE_HPP_
#define SRC_CPUFEATURE_HPP_

#include <string>

namespace Elpida
{

	class CpuFeature
	{
		public:

			const std::string& getName() const
			{
				return _name;
			}

			const std::string& getDescription() const
			{
				return _description;
			}

			bool isSupported() const
			{
				return _supported;
			}

			void setSupported(bool supported)
			{
				_supported = supported;
			}

			CpuFeature() :
					_supported(false)
			{

			}

			CpuFeature(const std::string& name, const std::string& description, bool supported = false) :
					_name(name),
					_description(description),
					_supported(supported)
			{

			}
			~CpuFeature()
			{

			}

			CpuFeature(CpuFeature&&) = default;
			CpuFeature(const CpuFeature&) = default;
			CpuFeature& operator=(CpuFeature&&) = default;
			CpuFeature& operator=(const CpuFeature&) = default;

		private:
			std::string _name;
			std::string _description;
			bool _supported;
	};

} /* namespace Elpida */

#endif /* SRC_CPUFEATURE_HPP_ */
