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
// Created by klapeto on 11/12/2023.
//

#ifndef ELPIDA_SVG_SVGPRESERVEASPECTRATIO_HPP
#define ELPIDA_SVG_SVGPRESERVEASPECTRATIO_HPP

#include <string_view>

namespace Elpida
{

	enum class SvgAlignType
	{
		Meet,
		Slice
	};

	enum class SvgAxisAlignType
	{
		None,
		Min,
		Mid,
		Max
	};

	class SvgPreserveAspectRatio
	{
	public:
		[[nodiscard]]
		SvgAlignType GetType() const
		{
			return _type;
		}

		[[nodiscard]]
		SvgAxisAlignType GetAlignX() const
		{
			return _alignX;
		}

		[[nodiscard]]
		SvgAxisAlignType GetAlignY() const
		{
			return _alignY;
		}

		SvgPreserveAspectRatio()
			: _type(SvgAlignType::Meet), _alignX(SvgAxisAlignType::Mid), _alignY(SvgAxisAlignType::Mid)
		{

		}
		SvgPreserveAspectRatio(SvgAlignType type, SvgAxisAlignType alignX, SvgAxisAlignType alignY)
			: _type(type), _alignX(alignX), _alignY(alignY)
		{
		}

		explicit SvgPreserveAspectRatio(std::string_view view);
	private:
		SvgAlignType _type;
		SvgAxisAlignType _alignX;
		SvgAxisAlignType _alignY;
	};

} // Elpida

#endif //ELPIDA_SVG_SVGPRESERVEASPECTRATIO_HPP
