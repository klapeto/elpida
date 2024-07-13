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
		const SvgPolygon& GetFillPolygon() const;

		[[nodiscard]]
		const SvgPolygon& GetStrokePolygon() const;

		[[nodiscard]]
		const std::vector<SvgPathInstance>& GetPaths() const;

		[[nodiscard]]
		const std::vector<SvgCalculatedShape>& GetChildren() const;

		[[nodiscard]]
		std::vector<SvgCalculatedShape>& GetChildren();

		[[nodiscard]]
		const SvgBounds& GetBounds() const;

		[[nodiscard]]
		const std::optional<SvgCalculatedFill>& GetFill() const;

		[[nodiscard]]
		const std::optional<SvgCalculatedStroke>& GetStroke() const;

		[[nodiscard]]
		std::optional<SvgCalculatedFill>& GetFill();

		[[nodiscard]]
		std::optional<SvgCalculatedStroke>& GetStroke();

		[[nodiscard]]
		double GetOpacity() const;

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
				double opacity,
				const SvgCalculationContext& calculationContext);
	private:
		std::vector<SvgPathInstance> _paths;
		std::vector<SvgCalculatedShape> _children;
		std::optional<SvgCalculatedFill> _fill;
		std::optional<SvgCalculatedStroke> _stroke;
		SvgBounds _bounds;
		double _opacity;
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
