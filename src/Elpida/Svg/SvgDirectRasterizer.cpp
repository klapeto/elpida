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
// Created by klapeto on 23/6/2024.
//

#include "Elpida/Svg/SvgDirectRasterizer.hpp"
#include "Elpida/Svg/SvgSuperSampler.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "Elpida/Svg/SvgBackDrop.hpp"

namespace Elpida
{

	static void RasterizeShape(const SvgCalculatedShape& shape,
			SvgBackDrop& finalBackdrop,
			const SvgSuperSampler& superSampler)
	{
		auto& fill = shape.GetFill();
		if (fill.has_value() && !shape.GetFillPolygon().GetEdges().empty())
		{
			finalBackdrop.Draw(shape.GetFillPolygon(), fill.value(), superSampler, fill->GetFillRule(), shape.BlendMode(), shape.CompositingMode(), shape.GetOpacity());
		}

		auto& stroke = shape.GetStroke();
		if (stroke.has_value() && !shape.GetStrokePolygon().GetEdges().empty())
		{
			finalBackdrop.Draw(shape.GetStrokePolygon(), stroke.value(), superSampler, SvgFillRule::NonZero, shape.BlendMode(), shape.CompositingMode(), shape.GetOpacity());
		}

		for (auto& child : shape.GetChildren())
		{
			RasterizeShape(child, finalBackdrop, superSampler);
		}
	}

	void SvgDirectRasterizer::Rasterize(const SvgCalculatedDocument& document,
			SvgBackDrop& backDrop,
			std::size_t subSamples)
	{
		const SvgSuperSampler superSampler(subSamples);

		Rasterize(document, backDrop, superSampler);
	}

	void SvgDirectRasterizer::Rasterize(const SvgCalculatedDocument& document, SvgBackDrop& backDrop,
			const SvgSuperSampler& superSampler)
	{
		backDrop.Clear();
		RasterizeShape(document.GetRootShape(), backDrop, superSampler);
	}
} // Elpida