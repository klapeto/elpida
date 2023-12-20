//
// Created by klapeto on 11/12/2023.
//

#include "Elpida/Svg/SvgPreserveAspectRatio.hpp"

#include "Elpida/Xml/CharacterStream.hpp"
#include "Elpida/Xml/ParseException.hpp"

namespace Elpida
{
	SvgPreserveAspectRatio::SvgPreserveAspectRatio(const std::string_view view)
	{
		CharacterStream stream(view);
		stream.SkipSpace();

		_type = SvgAlignType::Meet;
		_alignX = SvgAxisAlignType::Mid;
		_alignY = SvgAxisAlignType::Mid;

		if (stream.Current() == 'n')
		{
			if (!stream.ConsumeNextChars("none"))
			{
				throw ParseException("Unexpected character: expected 'none'");
			}
			_type = SvgAlignType::None;
			return;
		}

		if (!stream.ConsumeNextChars("xM"))
		{
			throw ParseException("Unexpected character: expected 'xMin' or 'xMax' or 'xMid'");
		}

		auto callback = [&stream](SvgAxisAlignType& align)
		{
			switch (stream.Current())
			{
			case 'i':
				if (!stream.Next())
				{
					throw ParseException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
				}
				switch (stream.Current())
				{
				case 'd':
					align = SvgAxisAlignType::Mid;
					break;
				case 'n':
					align = SvgAxisAlignType::Min;
					break;
				default:
					throw ParseException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
				}
				stream.Next();
				break;
			case 'a':
				if (!stream.Next())
				{
					throw ParseException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
				}
				if (stream.Current() != 'x')
				{
					throw ParseException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
				}
				align = SvgAxisAlignType::Max;
				stream.Next();
				break;
			default:
				throw ParseException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
			}
		};

		callback(_alignX);

		if (!stream.ConsumeNextChars("YM"))
		{
			throw ParseException("Unexpected character: expected 'YMin' or 'YMax' or 'YMid'");
		}

		callback(_alignY);

		stream.SkipSpace();
		if (!stream.Eof())
		{
			if (stream.Current() == 'm')
			{
				stream.Next();
				if (!stream.ConsumeNextChars("eet"))
				{
					throw ParseException("Unexpected character: expected 'meet'");
				}
				_type = SvgAlignType::Meet;
			}
			else if (stream.Current() == 's')
			{
				stream.Next();
				if (!stream.ConsumeNextChars("lice"))
				{
					throw ParseException("Unexpected character: expected 'meet'");
				}
				_type = SvgAlignType::Slice;
			}
			else
			{
				throw ParseException("Unexpected character: expected 'meet' or 'slice'");
			}
		}
	}
} // Elpida
