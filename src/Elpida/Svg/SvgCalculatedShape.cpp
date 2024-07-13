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

	std::vector<SvgCalculatedShape>& SvgCalculatedShape::GetChildren()
	{
		return _children;
	}

	std::optional<SvgCalculatedFill>& SvgCalculatedShape::GetFill()
	{
		return _fill;
	}

	std::optional<SvgCalculatedStroke>& SvgCalculatedShape::GetStroke()
	{
		return _stroke;
	}

	const SvgPolygon& SvgCalculatedShape::GetStrokePolygon() const
	{
		return _strokePolygon;
	}

	const SvgPolygon& SvgCalculatedShape::GetFillPolygon() const
	{
		return _fillPolygon;
	}

	const std::vector<SvgPathInstance>& SvgCalculatedShape::GetPaths() const
	{
		return _paths;
	}

} // Elpida
