//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgColor.hpp"

#include "Elpida/Svg/SvgNumber.hpp"
#include "Elpida/Xml/CharacterStream.hpp"

#include <unordered_map>

namespace Elpida
{
	static constexpr unsigned int RgbToInt(const unsigned char r, const unsigned char g, const unsigned char b)
	{
		return 255 << 24 | b << 16 | g << 8 | r;
	}

	SvgColor::SvgColor(const std::string_view view)
	{
		CharacterStream stream(view);
		try
		{
			auto valueParser = [&]
			{
				std::string v;
				stream.Next();
				v += stream.Current();
				return SvgNumber::StrTol16(v);
			};

			auto valueParser2 = [&]
			{
				std::string v;
				stream.Next();
				v += stream.Current();
				stream.Next();
				v += stream.Current();
				return SvgNumber::StrTol16(v);
			};

			auto nameParse = [](const std::string_view& name) -> unsigned
			{
				static std::unordered_map<std::string_view, unsigned int> map
				{
					{"red", RgbToInt(255, 0, 0)},
					{"green", RgbToInt(0, 128, 0)},
					{"blue", RgbToInt(0, 0, 255)},
					{"yellow", RgbToInt(255, 255, 0)},
					{"cyan", RgbToInt(0, 255, 255)},
					{"magenta", RgbToInt(255, 0, 255)},
					{"black", RgbToInt(0, 0, 0)},
					{"grey", RgbToInt(128, 128, 128)},
					{"gray", RgbToInt(128, 128, 128)},
					{"white", RgbToInt(255, 255, 255)},
					{"aliceblue", RgbToInt(240, 248, 255)},
					{"antiquewhite", RgbToInt(250, 235, 215)},
					{"aqua", RgbToInt(0, 255, 255)},
					{"aquamarine", RgbToInt(127, 255, 212)},
					{"azure", RgbToInt(240, 255, 255)},
					{"beige", RgbToInt(245, 245, 220)},
					{"bisque", RgbToInt(255, 228, 196)},
					{"blanchedalmond", RgbToInt(255, 235, 205)},
					{"blueviolet", RgbToInt(138, 43, 226)},
					{"brown", RgbToInt(165, 42, 42)},
					{"burlywood", RgbToInt(222, 184, 135)},
					{"cadetblue", RgbToInt(95, 158, 160)},
					{"chartreuse", RgbToInt(127, 255, 0)},
					{"chocolate", RgbToInt(210, 105, 30)},
					{"coral", RgbToInt(255, 127, 80)},
					{"cornflowerblue", RgbToInt(100, 149, 237)},
					{"cornsilk", RgbToInt(255, 248, 220)},
					{"crimson", RgbToInt(220, 20, 60)},
					{"darkblue", RgbToInt(0, 0, 139)},
					{"darkcyan", RgbToInt(0, 139, 139)},
					{"darkgoldenrod", RgbToInt(184, 134, 11)},
					{"darkgray", RgbToInt(169, 169, 169)},
					{"darkgreen", RgbToInt(0, 100, 0)},
					{"darkgrey", RgbToInt(169, 169, 169)},
					{"darkkhaki", RgbToInt(189, 183, 107)},
					{"darkmagenta", RgbToInt(139, 0, 139)},
					{"darkolivegreen", RgbToInt(85, 107, 47)},
					{"darkorange", RgbToInt(255, 140, 0)},
					{"darkorchid", RgbToInt(153, 50, 204)},
					{"darkred", RgbToInt(139, 0, 0)},
					{"darksalmon", RgbToInt(233, 150, 122)},
					{"darkseagreen", RgbToInt(143, 188, 143)},
					{"darkslateblue", RgbToInt(72, 61, 139)},
					{"darkslategray", RgbToInt(47, 79, 79)},
					{"darkslategrey", RgbToInt(47, 79, 79)},
					{"darkturquoise", RgbToInt(0, 206, 209)},
					{"darkviolet", RgbToInt(148, 0, 211)},
					{"deeppink", RgbToInt(255, 20, 147)},
					{"deepskyblue", RgbToInt(0, 191, 255)},
					{"dimgray", RgbToInt(105, 105, 105)},
					{"dimgrey", RgbToInt(105, 105, 105)},
					{"dodgerblue", RgbToInt(30, 144, 255)},
					{"firebrick", RgbToInt(178, 34, 34)},
					{"floralwhite", RgbToInt(255, 250, 240)},
					{"forestgreen", RgbToInt(34, 139, 34)},
					{"fuchsia", RgbToInt(255, 0, 255)},
					{"gainsboro", RgbToInt(220, 220, 220)},
					{"ghostwhite", RgbToInt(248, 248, 255)},
					{"gold", RgbToInt(255, 215, 0)},
					{"goldenrod", RgbToInt(218, 165, 32)},
					{"greenyellow", RgbToInt(173, 255, 47)},
					{"honeydew", RgbToInt(240, 255, 240)},
					{"hotpink", RgbToInt(255, 105, 180)},
					{"indianred", RgbToInt(205, 92, 92)},
					{"indigo", RgbToInt(75, 0, 130)},
					{"ivory", RgbToInt(255, 255, 240)},
					{"khaki", RgbToInt(240, 230, 140)},
					{"lavender", RgbToInt(230, 230, 250)},
					{"lavenderblush", RgbToInt(255, 240, 245)},
					{"lawngreen", RgbToInt(124, 252, 0)},
					{"lemonchiffon", RgbToInt(255, 250, 205)},
					{"lightblue", RgbToInt(173, 216, 230)},
					{"lightcoral", RgbToInt(240, 128, 128)},
					{"lightcyan", RgbToInt(224, 255, 255)},
					{"lightgoldenrodyellow", RgbToInt(250, 250, 210)},
					{"lightgray", RgbToInt(211, 211, 211)},
					{"lightgreen", RgbToInt(144, 238, 144)},
					{"lightgrey", RgbToInt(211, 211, 211)},
					{"lightpink", RgbToInt(255, 182, 193)},
					{"lightsalmon", RgbToInt(255, 160, 122)},
					{"lightseagreen", RgbToInt(32, 178, 170)},
					{"lightskyblue", RgbToInt(135, 206, 250)},
					{"lightslategray", RgbToInt(119, 136, 153)},
					{"lightslategrey", RgbToInt(119, 136, 153)},
					{"lightsteelblue", RgbToInt(176, 196, 222)},
					{"lightyellow", RgbToInt(255, 255, 224)},
					{"lime", RgbToInt(0, 255, 0)},
					{"limegreen", RgbToInt(50, 205, 50)},
					{"linen", RgbToInt(250, 240, 230)},
					{"maroon", RgbToInt(128, 0, 0)},
					{"mediumaquamarine", RgbToInt(102, 205, 170)},
					{"mediumblue", RgbToInt(0, 0, 205)},
					{"mediumorchid", RgbToInt(186, 85, 211)},
					{"mediumpurple", RgbToInt(147, 112, 219)},
					{"mediumseagreen", RgbToInt(60, 179, 113)},
					{"mediumslateblue", RgbToInt(123, 104, 238)},
					{"mediumspringgreen", RgbToInt(0, 250, 154)},
					{"mediumturquoise", RgbToInt(72, 209, 204)},
					{"mediumvioletred", RgbToInt(199, 21, 133)},
					{"midnightblue", RgbToInt(25, 25, 112)},
					{"mintcream", RgbToInt(245, 255, 250)},
					{"mistyrose", RgbToInt(255, 228, 225)},
					{"moccasin", RgbToInt(255, 228, 181)},
					{"navajowhite", RgbToInt(255, 222, 173)},
					{"navy", RgbToInt(0, 0, 128)},
					{"oldlace", RgbToInt(253, 245, 230)},
					{"olive", RgbToInt(128, 128, 0)},
					{"olivedrab", RgbToInt(107, 142, 35)},
					{"orange", RgbToInt(255, 165, 0)},
					{"orangered", RgbToInt(255, 69, 0)},
					{"orchid", RgbToInt(218, 112, 214)},
					{"palegoldenrod", RgbToInt(238, 232, 170)},
					{"palegreen", RgbToInt(152, 251, 152)},
					{"paleturquoise", RgbToInt(175, 238, 238)},
					{"palevioletred", RgbToInt(219, 112, 147)},
					{"papayawhip", RgbToInt(255, 239, 213)},
					{"peachpuff", RgbToInt(255, 218, 185)},
					{"peru", RgbToInt(205, 133, 63)},
					{"pink", RgbToInt(255, 192, 203)},
					{"plum", RgbToInt(221, 160, 221)},
					{"powderblue", RgbToInt(176, 224, 230)},
					{"purple", RgbToInt(128, 0, 128)},
					{"rosybrown", RgbToInt(188, 143, 143)},
					{"royalblue", RgbToInt(65, 105, 225)},
					{"saddlebrown", RgbToInt(139, 69, 19)},
					{"salmon", RgbToInt(250, 128, 114)},
					{"sandybrown", RgbToInt(244, 164, 96)},
					{"seagreen", RgbToInt(46, 139, 87)},
					{"seashell", RgbToInt(255, 245, 238)},
					{"sienna", RgbToInt(160, 82, 45)},
					{"silver", RgbToInt(192, 192, 192)},
					{"skyblue", RgbToInt(135, 206, 235)},
					{"slateblue", RgbToInt(106, 90, 205)},
					{"slategray", RgbToInt(112, 128, 144)},
					{"slategrey", RgbToInt(112, 128, 144)},
					{"snow", RgbToInt(255, 250, 250)},
					{"springgreen", RgbToInt(0, 255, 127)},
					{"steelblue", RgbToInt(70, 130, 180)},
					{"tan", RgbToInt(210, 180, 140)},
					{"teal", RgbToInt(0, 128, 128)},
					{"thistle", RgbToInt(216, 191, 216)},
					{"tomato", RgbToInt(255, 99, 71)},
					{"turquoise", RgbToInt(64, 224, 208)},
					{"violet", RgbToInt(238, 130, 238)},
					{"wheat", RgbToInt(245, 222, 179)},
					{"whitesmoke", RgbToInt(245, 245, 245)},
					{"yellowgreen", RgbToInt(154, 205, 50)},
				};

				const auto itr = map.find(name);
				if (itr != map.end())
				{
					return itr->second;
				}
				return 0;
			};

			unsigned long r = 0;
			unsigned long g = 0;
			unsigned long b = 0;

			stream.SkipSpace();
			switch (stream.Current())
			{
			case '#':
				{
					switch (stream.AvailableCharacters())
					{
					case 3:
						r = valueParser() * 17;
						g = valueParser() * 17;
						b = valueParser() * 17;
						break;
					case 6:
						r = valueParser2();
						g = valueParser2();
						b = valueParser2();
					default:
						break;
					}
				}
				break;
			case 'r':
				stream.Next();
				if (stream.ConsumeNextCharsCond("gb("))
				{
					auto value = stream.GetStringViewWhile([](auto c) { return c != ','; });
					if (value.empty())
					{
						throw ParseException("Unexpected character: expected digit");
					}
					r = SvgNumber::StrTol(value);
					stream.Next();
					value = stream.GetStringViewWhile([](auto c) { return c != ','; });
					if (value.empty())
					{
						throw ParseException("Unexpected character: expected digit");
					}
					g = SvgNumber::StrTol(value);
					stream.Next();
					value = stream.GetStringViewWhile([](auto c) { return c != ')'; });
					if (value.empty())
					{
						throw ParseException("Unexpected character: expected digit");
					}
					b = SvgNumber::StrTol(value);
				}
				else
				{
					_value = nameParse(view);
					return;
				}
				break;
			default:
				_value = nameParse(view);
				return;
			}

			_value = RgbToInt(r, g, b);
		}
		catch (const ParseException&)
		{
			_value = 0;
		}
	}
} // Elpida
