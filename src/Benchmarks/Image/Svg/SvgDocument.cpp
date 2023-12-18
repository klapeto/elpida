//
// Created by klapeto on 7/12/2023.
//

#include "SvgDocument.hpp"

#include "Benchmarks/Image/Xml/CharacterStream.hpp"
#include "Benchmarks/Image/Xml/XmlElement.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "SvgCoordinate.hpp"
#include "SvgGradient.hpp"
#include "SvgGradientStop.hpp"
#include "SvgPreserveAspectRatio.hpp"
#include "SvgTransform.hpp"
#include "SvgViewBox.hpp"
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "SvgNumber.hpp"

namespace Elpida
{
	static const std::string Empty;

	static const std::string& GetAttributeValue(const XmlElement& element, const char* name)
	{
		auto& attributes = element.GetAttributes();
		auto itr = attributes.find(name);
		if (itr != attributes.end())
		{
			return itr->second;
		}

		return Empty;
	}

	static bool IsNumber(const char c)
	{
		return (unsigned char)(c - '0') < 10;
	}

	static double ParseNumber(CharacterStream& stream)
	{
		stream.SkipSpace();
		double sign;

		switch (stream.Current())
		{
		case '-':
			sign = -1.0;
			if (!stream.Next())
			{
				throw ElpidaException("invalid character, expected number");
			}
			break;
		case '+':
			sign = 1.0;
			if (!stream.Next())
			{
				throw ElpidaException("invalid character, expected number");
			}
			break;
		default:
			sign = 1.0;
			break;
		}

		auto integerPart = stream.GetStringViewWhile([](auto c)
		{ return IsNumber(c); });

		if (stream.Current() == '.')
		{
			stream.Next();
		}

		auto floatPart = stream.GetStringViewWhile([](auto c)
		{ return IsNumber(c); });

		double expSign;

		std::string_view expPart;
		auto c = stream.Current();
		if (c == 'e' || c == 'E')
		{
			stream.Next();
			c = stream.Current();
			if (c == '+' || c == '-')
			{
				expSign = c == '+' ? 1.0 : -1.0;
				if (!stream.Next())
				{
					throw ElpidaException("Unexpected EOF. Expected exp number");
				}

				expPart = stream.GetStringViewWhile([](auto c)
				{ return IsNumber(c); });
			}
		}

		double value = SvgNumber::StrTol(integerPart);

		value += (double)(SvgNumber::StrTol(floatPart)) / pow(10.0, floatPart.length());

		if (!expPart.empty())
		{
			double expValue = SvgNumber::StrTol(expPart);
			value *= pow(10.0, expValue * expSign);
		}

		return value * sign;
	}

	static SvgUnits ParseUnits(CharacterStream& stream)
	{
		stream.SkipSpace();
		if (stream.Eof()) return SvgUnits::Px;

		auto callback = [&stream](char c, SvgUnits units)
		{
			if (!stream.Next())
			{
				throw ElpidaException("Unexpected EOF. Expected letter after");
			}
			if (stream.Current() != c)
			{
				throw ElpidaException("Unexpected character: expected '" + std::string() + c);
			}
			stream.Next();
			return units;
		};
		switch (stream.Current())
		{
		case 'p':
			if (!stream.Next())
			{
				throw ElpidaException("Unexpected EOF. Expected letter after 'p'");
			}
			switch (stream.Current())
			{
			case 'x':
				stream.Next();
				return SvgUnits::Px;
			case 't':
				stream.Next();
				return SvgUnits::Pt;
			case 'c':
				stream.Next();
				return SvgUnits::Pc;
			default:
				throw ElpidaException("Unexpected character: expected 'px', 'pt', 'pc'");
			}
		case 'm':
			return callback('m', SvgUnits::Mm);
		case 'c':
			return callback('m', SvgUnits::Cm);
		case 'i':
			return callback('n', SvgUnits::In);
		case '%':
			stream.Next();
			return SvgUnits::Percent;
		case 'e':
			if (!stream.Next())
			{
				throw ElpidaException("Unexpected EOF. Expected letter after 'e'");
			}
			switch (stream.Current())
			{
			case 'm':
				stream.Next();
				return SvgUnits::Em;
			case 'x':
				stream.Next();
				return SvgUnits::Ex;
			default:
				throw ElpidaException("Unexpected character: expected 'em', 'ex'");
			}
		default:
			throw ElpidaException("Unexpected character: expected unit");
		}
	}

	static SvgCoordinate ParseCoordinate(std::string_view view)
	{
		if (view.empty()) return SvgCoordinate{ 0, SvgUnits::Px };

		CharacterStream stream(view);
		return SvgCoordinate{ ParseNumber(stream), ParseUnits(stream) };
	}

	template<typename T>
	static void ConditionallyAssign(T& value, std::string_view view)
	{
		if (!view.empty())
		{
			value = T(view);
		}
	}

	template<std::size_t N>
	static void ParseTransformationValues(CharacterStream& stream, double (& values)[N])
	{
		std::size_t i = 0;
		while (!stream.Eof())
		{
			stream.SkipSpace();
			switch (stream.Current())
			{
			case ')':
				return;
			case '(':
			case ',':
				if (i == N)throw ElpidaException("Transforms must have max 6 comma separated numbers");
				stream.Next();
				values[i++] = ParseNumber(stream);
				continue;
			default:
				stream.Next();
				break;
			}
		}
		if (i != N - 1)
		{
			throw ElpidaException("Transform expected " + std::to_string(N) + " numbers");
		}
	}

	static SvgTransform ParseTransform(std::string_view view)
	{
		if (view.empty()) return {};

		CharacterStream stream(view);
		stream.SkipSpace();

		SvgTransform ret;
		SvgTransform transform;

		switch (stream.Current())
		{
		case 'm':
			if (!stream.ConsumeNextChars("matrix"))
			{
				throw ElpidaException("Unexpected character: expected 'matrix'");
			}
			{
				double t[6];
				ParseTransformationValues(stream, t);
				transform = SvgTransform(t);
			}
			break;
		case 't':
			if (!stream.ConsumeNextChars("translate"))
			{
				throw ElpidaException("Unexpected character: expected 'translate'");
			}
			{
				double t[2];
				ParseTransformationValues(stream, t);
				transform.SetTranslation(t[0], t[1]);
			}
			break;
		case 's':
			if (!stream.Next())
			{
				throw ElpidaException("Unexpected EOF: expected 'scale' or 'skewX' or 'skewY'");
			}
			if (stream.Current() == 'c')
			{
				if (!stream.ConsumeNextChars("cale"))
				{
					throw ElpidaException("Unexpected character: expected 'scale'");
				}

				double t[2];
				ParseTransformationValues(stream, t);
				transform.SetScale(t[0], t[1]);
			}
			else if (stream.Current() == 'k')
			{
				if (!stream.ConsumeNextChars("kew"))
				{
					throw ElpidaException("Unexpected character: expected 'skewX' or 'skewY'");
				}
				if (!stream.Next())
				{
					throw ElpidaException("Unexpected EOF: expected 'skewX' or 'skewY'");
				}
				if (stream.Current() == 'X')
				{
					double t[1];
					ParseTransformationValues(stream, t);
					transform.SetSkewX(t[0]);
				}
				else
				{
					double t[1];
					ParseTransformationValues(stream, t);
					transform.SetSkewY(t[0]);
				}
			}
			else
			{
				throw ElpidaException("Unexpected character: expected 'scale' or 'skewX' or 'skewY'");
			}
			break;
		}

		ret.PreMultiply(transform);
		return ret;
	}

	static SvgGradientStop ParseStop(const XmlElement& element)
	{
		auto& attributes = element.GetAttributes();

		SvgColor color;
		auto& value = GetAttributeValue(element, "stop-color");
		if (!value.empty())
		{

		}
	}

	struct BasicGradientData
	{
		std::string id;
		std::string href;
		std::vector<SvgGradientStop> stops;
		SvgTransform transform;
		SvgSpreadType spreadType = SvgSpreadType::Pad;
		SvgGradientUnits units = SvgGradientUnits::Object;
	};

	static BasicGradientData ParseBasicGradient(const XmlElement& element)
	{
		BasicGradientData basicData;

		{
			auto& value = GetAttributeValue(element, "gradientUnits");
			if (!value.empty())
			{
				basicData.units = value == "objectBoundingBox" ? SvgGradientUnits::Object : SvgGradientUnits::User;
			}
		}

		{
			auto& value = GetAttributeValue(element, "gradientTransform");
			if (!value.empty())
			{
				basicData.transform = ParseTransform(value);
			}
		}

		{
			auto& value = GetAttributeValue(element, "spreadMethod");
			if (!value.empty())
			{
				if (value == "pad")
				{
					basicData.spreadType = SvgSpreadType::Pad;
				}
				else if (value == "reflect")
				{
					basicData.spreadType = SvgSpreadType::Reflect;
				}
				else if (value == "repeat")
				{
					basicData.spreadType = SvgSpreadType::Repeat;
				}
				else
				{
					throw ElpidaException("Unexpected value: expected 'pad' or 'reflect' or 'repeat'");
				}
			}
		}

		basicData.stops.reserve(element.GetChildren().size());
		for (auto& child: element.GetChildren())
		{
			if (child.GetName() == "stop")
			{
				basicData.stops.push_back(ParseStop(child));
			}
		}
		std::sort(basicData.stops.begin(), basicData.stops.end(), [](SvgGradientStop& a, SvgGradientStop& b)
		{ return a.GetOffset() > b.GetOffset(); });
		basicData.stops.shrink_to_fit();

		basicData.id = GetAttributeValue(element, "id");
		basicData.href = GetAttributeValue(element, "xlink:href");

		return basicData;
	}

	static SvgGradient ParseLinearGradient(const XmlElement& element)
	{
		SvgCoordinate x1(0.0, SvgUnits::Percent);
		SvgCoordinate y1(0.0, SvgUnits::Percent);
		SvgCoordinate x2(100.0, SvgUnits::Percent);
		SvgCoordinate y2(0.0, SvgUnits::Percent);

		{
			auto& value = GetAttributeValue(element, "x1");
			if (!value.empty())
			{
				x1 = ParseCoordinate(value);
			}
		}

		{
			auto& value = GetAttributeValue(element, "y1");
			if (!value.empty())
			{
				y1 = ParseCoordinate(value);
			}
		}

		{
			auto& value = GetAttributeValue(element, "x2");
			if (!value.empty())
			{
				x2 = ParseCoordinate(value);
			}
		}

		{
			auto& value = GetAttributeValue(element, "y2");
			if (!value.empty())
			{
				y2 = ParseCoordinate(value);
			}
		}

		auto data = ParseBasicGradient(element);

		return SvgGradient(std::move(data.id),
				std::move(data.href),
				std::move(data.stops),
				data.transform,
				data.spreadType,
				data.units,
				x1,
				y1,
				x2,
				y2);
	}

	static void ParseDefs(const XmlElement& element, std::vector<SvgGradient>& gradients)
	{
		for (auto& child: element.GetChildren())
		{
			if (child.GetName() == "linearGradient")
			{
				gradients.push_back(ParseLinearGradient(child));
			}
		}
	}

	SvgDocument::SvgDocument(const XmlElement& element)
			: _width(0), _height(0)
	{
		if (element.GetName() != "svg")
		{
			throw ElpidaException("Element has invalid tag. It expected '<svg>' and got " + element.GetName());
		}

		_width = SvgCoordinate(GetAttributeValue(element, "width")).CalculatePixels(0, 0, 0, 0);
		_height = SvgCoordinate(GetAttributeValue(element, "height")).CalculatePixels(0, 0, 0, 0);
		ConditionallyAssign(_viewBox, GetAttributeValue(element, "viewBox"));
		ConditionallyAssign(_preserveAspectRatio, GetAttributeValue(element, "preserveAspectRatio"));

		for (auto& child: element.GetChildren())
		{
			if (child.GetName() == "defs")
			{
				ParseDefs(child, _gradients);
			}
		}
	}
} // Elpida