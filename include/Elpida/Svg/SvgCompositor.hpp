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

#ifndef ELPIDA_SVGCOMPOSITOR_HPP
#define ELPIDA_SVGCOMPOSITOR_HPP

#include "SvgColor.hpp"
#include "SvgCompositingMode.hpp"
#include "SvgConfig.hpp"

namespace Elpida
{
	class SvgCompositor
	{
	public:
		[[nodiscard]]
		SvgFloat Composite(const SvgFloat as, const SvgFloat Cs, const SvgFloat ab, const SvgFloat Cb) const
		{
			return _compositor(as, Cs, ab, Cb);
		}

		[[nodiscard]]
		SvgColor Composite(const SvgColor &Cs, const SvgColor &Cb) const
		{
			return {
				Composite(Cs.A(), Cs.R(), Cb.A(), Cb.R()),
				Composite(Cs.A(), Cs.G(), Cb.A(), Cb.G()),
				Composite(Cs.A(), Cs.B(), Cb.A(), Cb.B()),
				Composite(Cs.A(), 1.0, Cb.A(), 1.0)
			};
		}

		explicit SvgCompositor(SvgCompositingMode mode);

	private:
		using Compositor = SvgFloat (*)(SvgFloat, SvgFloat, SvgFloat, SvgFloat);
		Compositor _compositor;
	};
} // Elpida

#endif //ELPIDA_SVGCOMPOSITOR_HPP
