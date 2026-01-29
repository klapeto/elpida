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

#ifndef ELPIDA_SVG_SVGDOCUMENT_HPP
#define ELPIDA_SVG_SVGDOCUMENT_HPP

#include <stack>

#include "SvgDefs.hpp"
#include "SvgSvgElement.hpp"

namespace Elpida
{
	class XmlElement;

	class SvgDocument final
	{
	public:
		[[nodiscard]]
		const SvgDefs& GetDefs() const
		{
			return _defs;
		}

		[[nodiscard]]
		const SvgSvgElement& GetElement() const
		{
			return _element;
		}

		SvgDocument() = default;
		explicit SvgDocument(const XmlElement& element);
		SvgDocument(const SvgDocument&) = default;
		SvgDocument& operator=(const SvgDocument& other) = default;
		SvgDocument(SvgDocument&& other) noexcept = default;
		SvgDocument& operator=(SvgDocument&& other) noexcept = default;
		~SvgDocument() = default;
	private:
		SvgDefs _defs;
		SvgSvgElement _element;

		friend class SvgElement;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGDOCUMENT_HPP
