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
// Created by klapeto on 20/12/2023.
//

#ifndef ELPIDA_SVG_SVGSTYLE_HPP
#define ELPIDA_SVG_SVGSTYLE_HPP

#include <string>
#include <Elpida/Xml/XmlMap.hpp>

namespace Elpida
{
	class SvgStyle
	{
	public:
		[[nodiscard]]
		XmlMap& GetRules()
		{
			return _rules;
		}

		[[nodiscard]]
		const XmlMap& GetRules() const
		{
			return _rules;
		}

		SvgStyle() = default;
		explicit SvgStyle(std::string_view view);
	private:
		XmlMap _rules;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSTYLE_HPP
