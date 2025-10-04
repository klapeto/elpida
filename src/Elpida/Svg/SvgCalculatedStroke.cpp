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
// Created by klapeto on 9/3/2024.
//

#include "Elpida/Svg/SvgCalculatedStroke.hpp"

#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Svg/SvgStroke.hpp"

namespace Elpida
{
	SvgCalculatedStroke::SvgCalculatedStroke(const SvgStroke& stroke, const SvgBounds& elementBounds,
			const SvgDocument& document, const SvgCalculationContext& calculatingContext)
			:SvgCalculatedPaint(stroke, elementBounds, document, calculatingContext)
	{
		auto length = calculatingContext.GetViewBox().GetLength();

		for (auto& dash : stroke.GetDashes())
		{
			_dashes.push_back(dash.CalculateValue(calculatingContext, length));
		}

		_width = stroke.GetWidth().CalculateValue(calculatingContext, length);
		_dashOffset = stroke.GetDashOffset().CalculateValue(calculatingContext, length);
		_miterLimit = stroke.GetMiterLimit();
		_lineCap = stroke.GetLineCap();
		_lineJoin = stroke.GetLineJoin();
	}

	void SvgCalculatedStroke::Transform(const SvgTransform& transform)
	{
		SvgCalculatedPaint::Transform(transform);

		auto value= (transform[SvgTransform::A] + transform[SvgTransform::D]) / SvgFloat(2.0);
		_width *=  value;

		for (SvgFloat & dash : _dashes)
		{
			dash *= value;
		}

		_dashOffset *= value;
	}

} // Elpida