//
// Created by klapeto on 8/3/2024.
//

#include "Elpida/Svg/SvgCalculatedShape.hpp"

#include <Elpida/Xml/CharacterStream.hpp>

#include "Elpida/Svg/SvgFill.hpp"
#include "Elpida/Svg/SvgStroke.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"

namespace Elpida
{
	void SvgCalculatedShape::RecalculateBounds()
	{
		_bounds = SvgBounds::CreateMinimum();

		for (auto &path: _paths)
		{
			_bounds.Merge(path.GetBounds());
		}

		for (auto &child: _children)
		{
			_bounds.Merge(child.GetBounds());
		}
	}

	const std::vector<SvgPathInstance> &SvgCalculatedShape::GetPaths() const
	{
		return _paths;
	}

	const std::vector<SvgCalculatedShape> &SvgCalculatedShape::GetChildren() const
	{
		return _children;
	}

	const SvgBounds &SvgCalculatedShape::GetBounds() const
	{
		return _bounds;
	}

	static SvgBlendMode ParseBlendMode(std::string_view view)
	{
		if (view.empty()) return SvgBlendMode::Normal;

		CharacterStream stream(view);

		stream.SkipSpace();

		switch (stream.Current())
		{
			case 'n':
				stream.Next();
				if (stream.ConsumeNextChars("ormal"))
				{
					return SvgBlendMode::Normal;
				}
				break;
			case 'm':
				stream.Next();
				if (stream.ConsumeNextChars("ultiply"))
				{
					return SvgBlendMode::Multiply;
				}
				break;
			case 's':
				stream.Next();
				if (stream.ConsumeNextChars("creen"))
				{
					return SvgBlendMode::Screen;
				}
				break;
			case 'o':
				stream.Next();
				if (stream.ConsumeNextChars("verlay"))
				{
					return SvgBlendMode::Overlay;
				}
				break;
			case 'd':
				stream.Next();
				switch (stream.Current())
				{
					case 'a':
						stream.Next();
						if (stream.ConsumeNextChars("rken"))
						{
							return SvgBlendMode::Darken;
						}
						break;
					case 'i':
						stream.Next();
						if (stream.ConsumeNextChars("fference"))
						{
							return SvgBlendMode::Difference;
						}
						break;
					default: break;
				}
				break;
			case 'l':
				stream.Next();
				if (stream.ConsumeNextChars("ighten"))
				{
					return SvgBlendMode::Lighten;
				}
				break;

			case 'c':
				stream.Next();
				if (stream.ConsumeNextChars("olor-"))
				{
					switch (stream.Current())
					{
						case 'd':
							stream.Next();
							if (stream.ConsumeNextChars("odge"))
							{
								return SvgBlendMode::ColorDodge;
							}
							break;
						case 'b':
							stream.Next();
							if (stream.ConsumeNextChars("urn"))
							{
								return SvgBlendMode::ColorBurn;
							}
							break;
						default: break;
					}
				}
				break;
			default:
				return SvgBlendMode::Normal;
		}
	}

	SvgCalculatedShape::SvgCalculatedShape(std::vector<SvgPathInstance> &&paths,
	                                       const SvgFill &fill,
	                                       const SvgStroke &stroke,
	                                       const SvgDocument &document,
	                                       double opacity,
	                                       const SvgCalculationContext &calculationContext)
		: _paths(std::move(paths)), _opacity(opacity), _compositingMode(SvgCompositingMode::SourceOver),
		  _blendMode(SvgBlendMode::Normal)
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

		_blendMode = ParseBlendMode(calculationContext.GetValue("mix-blend-mode"));
	}

	const std::optional<SvgCalculatedFill> &SvgCalculatedShape::GetFill() const
	{
		return _fill;
	}

	const std::optional<SvgCalculatedStroke> &SvgCalculatedShape::GetStroke() const
	{
		return _stroke;
	}

	double SvgCalculatedShape::GetOpacity() const
	{
		return _opacity;
	}

	SvgCalculatedShape::SvgCalculatedShape()
		: _opacity(1.0)
	{
	}

	void SvgCalculatedShape::Transform(const SvgTransform &transform)
	{
		for (auto &path: _paths)
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

		for (auto &child: _children)
		{
			child.Transform(transform);
		}
		RecalculateBounds();
	}
} // Elpida
