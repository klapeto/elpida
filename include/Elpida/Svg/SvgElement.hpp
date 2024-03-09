//
// Created by klapeto on 20/12/2023.
//

#ifndef ELPIDA_SVG_SVGELEMENT_HPP
#define ELPIDA_SVG_SVGELEMENT_HPP

#include "SvgFill.hpp"
#include "SvgPathInstance.hpp"
#include "SvgStroke.hpp"
#include "Elpida/Svg/SvgTransform.hpp"
#include "Elpida/Xml/XmlElement.hpp"
#include "SvgPath.hpp"
#include "SvgRectangle.hpp"
#include "SvgCalculatedShape.hpp"

#include <variant>

namespace Elpida
{
	class SvgDocument;
	class SvgCalculationContext;

	enum class SvgShapeType
	{
		NonShape,
		Path,
		Rectangle
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

		[[nodiscard]]
		SvgCalculatedShape CalculateShape(const SvgDocument& document, SvgCalculationContext& calculationContext) const;

		SvgElement();
		explicit SvgElement(const XmlElement& element, SvgDocument& document);
		SvgElement(const SvgElement&) = delete;
		SvgElement(SvgElement&& other) noexcept = default;
		SvgElement& operator=(SvgElement&&) noexcept = default;
		virtual ~SvgElement() = default;

	private:
		std::string _id;
		SvgTransform _transform;
		XmlMap _properties;
		std::vector<SvgElement> _children;
		SvgFill _fill;
		SvgStroke _stroke;
		double _opacity;
		bool _visible;
		SvgShapeType _shapeType;
		std::variant<std::monostate, SvgPath, SvgRectangle> _shape;

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
