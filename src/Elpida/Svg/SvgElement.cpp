//
// Created by klapeto on 20/12/2023.
//

#include "Elpida/Svg/SvgElement.hpp"

#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgPathGenerator.hpp>

#include "Elpida/Svg/SvgDefs.hpp"
#include "Elpida/Svg/SvgNumber.hpp"
#include "Elpida/Svg/SvgStyle.hpp"
#include "Elpida/Svg/SvgVisibility.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"

namespace Elpida
{
	SvgElement::SvgElement(const XmlElement& element, SvgDocument& document)
			: _opacity(1.0), _visible(true), _shapeType(SvgShapeType::NonRenderable)
	{
		_properties = element.GetAttributes();

		SvgStyle style(element.GetAttributeValue("style"));
		for (auto& [key, value]: style.GetRules())
		{
			_properties.Set(key, std::move(value));
		}
		ConditionallyAssignProperty<>("id", _id);
		ConditionallyAssignProperty<>("transform", _transform);

		ConditionallyAssignProperty<>("opacity",
				_opacity,
				[](const auto& s)
				{
					SvgFloat v;
					return SvgNumber::TryParseNumber(s, v) ? v : 1.0;
				});
		ConditionallyAssignProperty<>("display",
				_visible,
				[](const auto& s)
				{
					return static_cast<bool>(SvgVisibility(s));
				});

		_fill = SvgFill(GetProperties());
		_stroke = SvgStroke(GetProperties());

		_name = element.GetName();

		if (_name == "path")
		{
			_shapeType = SvgShapeType::Path;
			_shape = SvgPath(_properties);
		}
		else if (_name == "g")
		{
			_shapeType = SvgShapeType::Group;
		}
		else if (_name == "svg")
		{
			_shapeType = SvgShapeType::Svg;
		}
		else if (_name == "rect")
		{
			_shapeType = SvgShapeType::Rectangle;
			_shape = SvgRectangle(_properties);
		}
		else if (_name == "circle")
		{
			_shapeType = SvgShapeType::Circle;
			_shape = SvgCircle(_properties);
		}
		else if (_name == "ellipse")
		{
			_shapeType = SvgShapeType::Ellipse;
			_shape = SvgEllipse(_properties);
		}
		else if (_name == "line")
		{
			_shapeType = SvgShapeType::Line;
			_shape = SvgLine(_properties);
		}

		auto& defs = document._defs;
		for (auto& child: element.GetChildren())
		{
			if (child.GetName() == "defs")
			{
				for (auto& def: child.GetChildren())
				{
					auto& id = def.GetAttributeValue("id");
					if (id.empty()) continue;
					if (def.GetName() == "linearGradient" || def.GetName() == "radialGradient")
					{
						defs.insert({ id, SvgGradient(def, document) });
					}
				}
				continue;
			}

			_children.emplace_back(child, document);
		}
	}

	SvgCalculatedShape SvgElement::CalculateShape(const SvgDocument& document,
			SvgCalculationContext& calculationContext) const
	{
		calculationContext.Push(*this);
		std::vector<SvgPathInstance> paths;
		switch (_shapeType)
		{
		case SvgShapeType::NonRenderable:
			break;
		case SvgShapeType::Path:
			paths = std::get<SvgPath>(_shape).CalculatePaths();
			break;
		case SvgShapeType::Rectangle:
			paths = std::get<SvgRectangle>(_shape).CalculatePaths(calculationContext);
			break;
		case SvgShapeType::Circle:
			paths = std::get<SvgCircle>(_shape).CalculatePaths(calculationContext);
			break;
		case SvgShapeType::Ellipse:
			paths = std::get<SvgEllipse>(_shape).CalculatePaths(calculationContext);
			break;
		case SvgShapeType::Line:
			paths = std::get<SvgLine>(_shape).CalculatePaths(calculationContext);
			break;
		default:
			break;
		}

		auto stackedOpacity = calculationContext.GetOpacity();
		auto calculatedInstance = SvgCalculatedShape(std::move(paths), calculationContext.GetFill(), calculationContext.GetStroke(), document, stackedOpacity,
				calculationContext);

		auto transform = calculationContext.CalculateTransform();
		if (!transform.IsIdentity())
		{
			calculatedInstance.Transform(transform);
		}

		std::vector<SvgCalculatedShape> children;
		children.reserve(_children.size());
		for (auto& child: _children)
		{
			if (child.GetShapeType() != SvgShapeType::NonRenderable && child.IsVisible())
			{
				children.push_back(child.CalculateShape(document, calculationContext));
			}
		}
		calculatedInstance.AddChildren(std::move(children));

		calculationContext.Pop();
		return calculatedInstance;
	}

	SvgElement::SvgElement()
			: _opacity(1.0), _visible(false), _shapeType(SvgShapeType::NonRenderable)
	{

	}
} // Elpida