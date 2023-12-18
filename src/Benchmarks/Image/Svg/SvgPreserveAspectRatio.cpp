//
// Created by klapeto on 11/12/2023.
//

#include "SvgPreserveAspectRatio.hpp"

#include <Benchmarks/Image/Xml/CharacterStream.hpp>
#include <Elpida/Core/ElpidaException.hpp>

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
				throw ElpidaException("Unexpected character: expected 'none'");
			}
			_type = SvgAlignType::None;
			return;
		}

		if (!stream.ConsumeNextChars("xM"))
		{
			throw ElpidaException("Unexpected character: expected 'xMin' or 'xMax' or 'xMid'");
		}

		auto callback = [&stream](SvgAxisAlignType& align)
		{
			switch (stream.Current())
			{
			case 'i':
				if (!stream.Next())
				{
					throw ElpidaException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
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
					throw ElpidaException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
				}
				stream.Next();
				break;
			case 'a':
				if (!stream.Next())
				{
					throw ElpidaException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
				}
				if (stream.Current() != 'x')
				{
					throw ElpidaException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
				}
				align = SvgAxisAlignType::Max;
				stream.Next();
				break;
			default:
				throw ElpidaException("Unexpected EOF: expected 'xMin' or 'xMax' or 'xMid'");
			}
		};

		callback(_alignX);

		if (!stream.ConsumeNextChars("YM"))
		{
			throw ElpidaException("Unexpected character: expected 'YMin' or 'YMax' or 'YMid'");
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
					throw ElpidaException("Unexpected character: expected 'meet'");
				}
				_type = SvgAlignType::Meet;
			}
			else if (stream.Current() == 's')
			{
				stream.Next();
				if (!stream.ConsumeNextChars("lice"))
				{
					throw ElpidaException("Unexpected character: expected 'meet'");
				}
				_type = SvgAlignType::Slice;
			}
			else
			{
				throw ElpidaException("Unexpected character: expected 'meet' or 'slice'");
			}
		}
	}
} // Elpida
