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
// Created by klapeto on 7/12/2023.
//

#ifndef ELPIDA_SVG_SVGGRADIENTSTOP_HPP
#define ELPIDA_SVG_SVGGRADIENTSTOP_HPP

#include "SvgColor.hpp"
#include "SvgLength.hpp"
#include "SvgElement.hpp"

namespace Elpida
{
	class XmlElement;

	class SvgGradientStop : public SvgElement
	{
	public:
		[[nodiscard]]
		const SvgColor& GetColor() const
		{
			return _color;
		}

		[[nodiscard]]
		const SvgLength& GetOffset() const
		{
			return _offset;
		}

		[[nodiscard]]
		const SvgLength& GetGradientOpacity() const
		{
			return _opacity;
		}

		explicit SvgGradientStop(const XmlElement& element, SvgDocument& document);
	private:
		SvgColor _color;
		SvgLength _offset;
		SvgLength _opacity;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGGRADIENTSTOP_HPP
