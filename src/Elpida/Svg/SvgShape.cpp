//
// Created by klapeto on 21/12/2023.
//

#include "Elpida/Svg/SvgShape.hpp"

#include "Elpida/Svg/SvgNumber.hpp"
#include "Elpida/Svg/SvgVisibility.hpp"

namespace Elpida
{
	SvgShape::SvgShape(const XmlElement& element, SvgDocument& document)
		: SvgElement(element, document), _opacity(1.0), _visible(true)
	{
		ConditionallyAssignProperty<>("opacity", _opacity, [](const auto& s)
		{
			double v;
			return SvgNumber::TryParseNumber(s, v) ? v : 1.0;
		});
		ConditionallyAssignProperty<>("display", _visible, [](const auto& s)
		{
			return static_cast<bool>(SvgVisibility(s));
		});

		ConditionallyAssignProperty<>("fill", _fill, [this](const auto& s)
		{
			CharacterStream stream(s);

			SvgColor color;
			std::string gradientId;

			stream.SkipSpace();
			switch (stream.Current())
			{
			case 'u':
				if (stream.ConsumeNextCharsCond("url("))
				{
					gradientId = stream.GetStringViewWhile([](auto c) { return c != ')'; });
				}
				else
				{
					color = SvgColor(stream.GetStringView());
				}
				break;
			case 'n':
				if (stream.ConsumeNextCharsCond("none"))
				{
					return std::nullopt;
				}
				else
				{
					color = SvgColor(stream.GetStringView());
				}
				break;
			default:
				color = SvgColor(stream.GetStringView());
				break;
			}

			std::string fillOpacity;
			ConditionallyAssignProperty<>("fill-opacity", fillOpacity);
		});
	}
} // Elpida
