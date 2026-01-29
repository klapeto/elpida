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
// Created by klapeto on 7/3/2024.
//

#ifndef ELPIDA_SVGBLENDER_HPP
#define ELPIDA_SVGBLENDER_HPP

#include "Elpida/Svg/SvgBlendMode.hpp"
#include "Elpida/Svg/SvgColor.hpp"
#include "Elpida/Svg/SvgConfig.hpp"

namespace Elpida
{

	class SvgBlender final
	{
	public:
		[[nodiscard]]
		SvgFloat Blend(const SvgFloat Cb, const SvgFloat Cs) const
		{
			return _blender(Cb, Cs);
		}

		[[nodiscard]]
		SvgFloat Blend(const SvgFloat Cs, const SvgFloat Cb, const SvgFloat ab) const
		{
			return (SvgFloat(1.0) - ab) * Cs + ab * Blend(Cb, Cs);
		}

		[[nodiscard]]
		SvgColor Blend(const SvgColor& Cs, const SvgColor& Cb) const
		{
			return {
				Blend(Cs.R(), Cb.R(), Cb.A()),
				Blend(Cs.G(), Cb.G(), Cb.A()),
				Blend(Cs.B(), Cb.B(), Cb.A()),
				Cs.A()
			};
		}

		explicit SvgBlender(SvgBlendMode mode);

	private:
		using Blender = SvgFloat (*)(SvgFloat, SvgFloat);
		Blender _blender;
	};

} // Elpida

#endif //ELPIDA_SVGBLENDER_HPP
