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

#ifndef ELPIDA_SVGCALCULATEDSHAPE_HPP
#define ELPIDA_SVGCALCULATEDSHAPE_HPP

#include "Elpida/Svg/SvgPathInstance.hpp"
#include "Elpida/Svg/SvgCalculatedPaint.hpp"
#include "Elpida/Svg/SvgCalculatedStroke.hpp"
#include "Elpida/Svg/SvgCalculatedFill.hpp"
#include "Elpida/Svg/SvgPolygon.hpp"
#include "Elpida/Svg/SvgConfig.hpp"

#include <vector>
#include <optional>

#include "SvgBlendMode.hpp"
#include "SvgCompositingMode.hpp"

namespace Elpida
{
	class SvgFill;
	class SvgStroke;

	class SvgCalculatedShape
	{
	public:

		[[nodiscard]]
		const SvgPolygon& GetFillPolygon() const
		{
			return _fillPolygon;
		}

		[[nodiscard]]
		const SvgPolygon& GetStrokePolygon() const
		{
			return _strokePolygon;
		}

		[[nodiscard]]
		const std::vector<SvgPathInstance>& GetPaths() const
		{
			return _paths;
		}

		[[nodiscard]]
		const std::vector<SvgCalculatedShape>& GetChildren() const
		{
			return _children;
		}

		[[nodiscard]]
		std::vector<SvgCalculatedShape>& GetChildren()
		{
			return _children;
		}

		[[nodiscard]]
		const SvgBounds& GetBounds() const
		{
			return _bounds;
		}

		[[nodiscard]]
		const std::optional<SvgCalculatedFill>& GetFill() const
		{
			return _fill;
		}

		[[nodiscard]]
		const std::optional<SvgCalculatedStroke>& GetStroke() const
		{
			return _stroke;
		}

		[[nodiscard]]
		std::optional<SvgCalculatedFill>& GetFill()
		{
			return _fill;
		}

		[[nodiscard]]
		std::optional<SvgCalculatedStroke>& GetStroke()
		{
			return _stroke;
		}

		[[nodiscard]]
		SvgFloat GetOpacity() const
		{
			return _opacity;
		}

		[[nodiscard]]
		SvgCompositingMode CompositingMode() const
		{
			return _compositingMode;
		}

		[[nodiscard]]
		SvgBlendMode BlendMode() const
		{
			return _blendMode;
		}

		void Transform(const SvgTransform& transform);

		SvgCalculatedShape();
		explicit SvgCalculatedShape(std::vector<SvgPathInstance>&& paths,
				const SvgFill& fill,
				const SvgStroke& stroke,
				const SvgDocument& document,
				SvgFloat opacity,
				const SvgCalculationContext& calculationContext);
	private:
		std::vector<SvgPathInstance> _paths;
		std::vector<SvgCalculatedShape> _children;
		std::optional<SvgCalculatedFill> _fill;
		std::optional<SvgCalculatedStroke> _stroke;
		SvgBounds _bounds;
		SvgFloat _opacity;
		SvgCompositingMode _compositingMode;
		SvgBlendMode _blendMode;
		SvgPolygon _fillPolygon;
		SvgPolygon _strokePolygon;

		void Recalculate();

		void AddChildren(std::vector<SvgCalculatedShape>&& children)
		{
			_children = std::move(children);
			Recalculate();
		}

		friend class SvgElement;
	};

} // Elpida

#endif //ELPIDA_SVGCALCULATEDSHAPE_HPP
