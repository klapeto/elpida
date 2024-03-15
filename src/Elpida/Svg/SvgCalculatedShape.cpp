//
// Created by klapeto on 8/3/2024.
//

#include "Elpida/Svg/SvgCalculatedShape.hpp"
#include "Elpida/Svg/SvgFill.hpp"
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
			const SvgFill& fill,
			const SvgStroke& stroke,
			const SvgDocument& document,
			double opacity,
			const SvgCalculationContext& calculationContext)
			:_paths(std::move(paths)), _opacity(opacity)
	{
		RecalculateBounds();

		if (fill.IsSet())
		{
			_fill = SvgCalculatedFill(fill, _bounds, document, calculationContext);
		}

		if (stroke.IsSet())
		{
			_stroke = SvgCalculatedStroke(stroke, _bounds, document, calculationContext);
		}
	}

	const std::optional<SvgCalculatedFill>& SvgCalculatedShape::GetFill() const
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
		RecalculateBounds();
	}

} // Elpida