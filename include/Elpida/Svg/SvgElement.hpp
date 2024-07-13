//
// Created by klapeto on 20/12/2023.
//

#ifndef ELPIDA_SVG_SVGELEMENT_HPP
#define ELPIDA_SVG_SVGELEMENT_HPP

#include "Elpida/Svg/SvgCalculatedShape.hpp"
#include "Elpida/Svg/SvgFill.hpp"
#include "Elpida/Svg/SvgPath.hpp"
#include "Elpida/Svg/SvgRectangle.hpp"
#include "Elpida/Svg/SvgStroke.hpp"
#include "Elpida/Svg/SvgTransform.hpp"
#include "Elpida/Xml/XmlElement.hpp"
#include "Elpida/Svg/SvgCircle.hpp"
#include "Elpida/Svg/SvgEllipse.hpp"
#include "Elpida/Svg/SvgLine.hpp"

#include <variant>

namespace Elpida
{
	class SvgDocument;
	class SvgCalculationContext;

	enum class SvgShapeType
	{
		NonRenderable,
		Svg,
		Group,
		Path,
		Rectangle,
		Circle,
		Ellipse,
		Line
	};

	class SvgElement
	{
	public:
		[[nodiscard]]
		const std::string& GetId() const
		{
			return _id;
		}

		[[nodiscard]]
		const std::string& GetName() const
		{
			return _name;
		}

		[[nodiscard]]
		const SvgTransform& GetTransform() const
		{
			return _transform;
		}

		[[nodiscard]]
		const XmlMap& GetProperties() const
		{
			return _properties;
		}

		[[nodiscard]]
		const std::vector<SvgElement>& GetChildren() const
		{
			return _children;
		}

		[[nodiscard]]
		const SvgFill& GetFill() const
		{
			return _fill;
		}

		[[nodiscard]]
		const SvgStroke& GetStroke() const
		{
			return _stroke;
		}

		[[nodiscard]]
		double GetOpacity() const
		{
			return _opacity;
		}

		[[nodiscard]]
		bool IsVisible() const
		{
			return _visible;
		}

		SvgShapeType GetShapeType() const;

		[[nodiscard]]
		SvgCalculatedShape CalculateShape(const SvgDocument& document, SvgCalculationContext& calculationContext) const;

		SvgElement();
		explicit SvgElement(const XmlElement& element, SvgDocument& document);
		SvgElement(const SvgElement&) = default;
		SvgElement(SvgElement&& other) noexcept = default;
		SvgElement& operator=(SvgElement&&) noexcept = default;
		SvgElement& operator=(const SvgElement&) = default;
		virtual ~SvgElement() = default;

	private:
		std::string _id;
		std::string _name;
		SvgTransform _transform;
		XmlMap _properties;
		std::vector<SvgElement> _children;
		SvgFill _fill;
		SvgStroke _stroke;
		double _opacity;
		bool _visible;
		SvgShapeType _shapeType;
		std::variant<std::monostate, SvgPath, SvgRectangle, SvgCircle, SvgEllipse, SvgLine> _shape;

	protected:
		template<typename T, typename TConverter>
		void ConditionallyAssignProperty(const std::string& name,
				T& targetValue,
				TConverter converter)
		{
			if (auto& value = _properties.GetValue(name); !value.empty())
			{
				targetValue = std::move(converter(value));
			}
		}

		template<typename T>
		void ConditionallyAssignProperty(const std::string& name, T& targetValue)
		{
			ConditionallyAssignProperty<T>(name, targetValue, [](const auto& s)
			{ return T(s); });
		}
	};
} // Elpida

#endif //ELPIDA_SVG_SVGELEMENT_HPP
