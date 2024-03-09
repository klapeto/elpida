//
// Created by klapeto on 8/3/2024.
//

#include "Elpida/Svg/SvgCalculatedShape.hpp"
#include "Elpida/Svg/SvgPaint.hpp"
#include "Elpida/Svg/SvgStroke.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"

namespace Elpida
{
	void SvgCalculatedShape::RecalculateBounds()
	{
		SvgBounds bounds = SvgBounds::CreateMinimum();

		for (auto& path : _paths)
		{
			bounds.Merge(path.GetBounds());
		}

		for (auto& child : _children)
		{
			bounds.Merge(child.GetBounds());
		}
	}

	const std::vector<SvgPathInstance>& SvgCalculatedShape::GetPaths() const
	{
		return _paths;
	}

	const std::vector<SvgCalculatedShape>& SvgCalculatedShape::GetChildren() const
	{
		return _children;
	}

	const SvgBounds& SvgCalculatedShape::GetBounds() const
	{
		return _bounds;
	}

	SvgCalculatedShape::SvgCalculatedShape(std::vector<SvgPathInstance>&& paths,
			const SvgPaint& fill,
			const SvgStroke& stroke,
			const SvgDocument& document,
			double opacity,
			const SvgCalculationContext& calculationContext)
			:_paths(std::move(paths)), _opacity(opacity)
	{
		RecalculateBounds();

		if (fill.IsSet())
		{
			_fill = SvgCalculatedPaint(fill, _bounds, document, calculationContext);
		}

		if (stroke.IsSet())
		{
			_stroke = SvgCalculatedStroke(stroke, _bounds, document, calculationContext);
		}
	}

	const std::optional<SvgCalculatedPaint>& SvgCalculatedShape::GetFill() const
	{
		return _fill;
	}

	const std::optional<SvgCalculatedStroke>& SvgCalculatedShape::GetStroke() const
	{
		return _stroke;
	}

	double SvgCalculatedShape::GetOpacity() const
	{
		return _opacity;
	}

	SvgCalculatedShape::SvgCalculatedShape()
			:_opacity(1.0)
	{

	}

} // Elpida