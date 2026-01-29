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
// Created by klapeto on 8/3/2024.
//

#include "Elpida/Svg/SvgCalculatedShape.hpp"

#include <Elpida/Xml/CharacterStream.hpp>

#include "Elpida/Svg/SvgFill.hpp"
#include "Elpida/Svg/SvgStroke.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Svg/SvgShapePolygonizer.hpp"

namespace Elpida
{
	void SvgCalculatedShape::Recalculate()
	{
		_bounds = SvgBounds::CreateMinimum();

		for (auto& child : _children)
		{
			_bounds.Merge(child.GetBounds());
		}

		if (_fill.has_value())
		{
			_fillPolygon = SvgShapePolygonizer::Polygonize(*this);
			_bounds.Merge(_fillPolygon.GetBounds());
		}

		if (_stroke.has_value())
		{
			_strokePolygon = SvgShapePolygonizer::PolygonizeStroke(*this);
			_bounds.Merge(_strokePolygon.GetBounds());
		}
	}

	SvgCalculatedShape::SvgCalculatedShape(std::vector<SvgPathInstance>&& paths,
			const SvgFill& fill,
			const SvgStroke& stroke,
			const SvgDocument& document,
			SvgFloat opacity,
			const SvgCalculationContext& calculationContext)
			:_paths(std::move(paths)), _opacity(opacity), _compositingMode(SvgCompositingMode::SourceOver),
			 _blendMode(SvgBlendMode::Normal)
	{
		Recalculate();

		if (fill.IsSet())
		{
			_fill = SvgCalculatedFill(fill, _bounds, document, calculationContext);
		}

		if (stroke.IsSet())
		{
			_stroke = SvgCalculatedStroke(stroke, _bounds, document, calculationContext);
		}

		_blendMode = SvgBlendModeParser::Parse(calculationContext.GetValue("mix-blend-mode"));
	}

	SvgCalculatedShape::SvgCalculatedShape()
			:_opacity(1.0), _compositingMode(SvgCompositingMode::SourceOver), _blendMode(SvgBlendMode::Normal)
	{
	}

	void SvgCalculatedShape::Transform(const SvgTransform& transform)
	{
		for (auto& path : _paths)
		{
			path.Transform(transform);
		}

		if (_fill.has_value())
		{
			_fill->Transform(transform);
		}

		if (_stroke.has_value())
		{
			_stroke->Transform(transform);
		}

		for (auto& child : _children)
		{
			child.Transform(transform);
		}
		Recalculate();
	}

} // Elpida
