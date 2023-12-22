//
// Created by klapeto on 20/12/2023.
//

#include "Elpida/Svg/SvgStyle.hpp"

#include "Elpida/Xml/CharacterStream.hpp"
#include "Elpida/Xml/ParseException.hpp"

namespace Elpida
{
	SvgStyle::SvgStyle(const std::string_view view)
	{
		CharacterStream stream(view);
		while (!stream.Eof())
		{
			stream.SkipSpace();
			auto name = stream.GetStringViewWhile([](auto c){return !CharacterStream::IsSpace(c) && c != ':';});
			stream.SkipSpace();
			if (stream.Current() != ':')
			{
				throw ParseException(stream.Current(), ":");
			}
			if (!stream.Next())
			{
				throw ParseException("Eof", "css rule value");
			}
			stream.SkipSpace();

			const auto firstChar = stream.Index();
			auto lastChar = stream.Index();
			while (!stream.Eof())
			{
				if (stream.Current() == ':')
				{
					throw ParseException(stream.Current(), "css rule value");
				}

				if (stream.Current() == ';')
				{
					break;
				}

				if (!CharacterStream::IsSpace(stream.Current()))
				{
					lastChar = stream.Index();
				}
				stream.Next();
			}
			if (lastChar - firstChar <= 0)
			{
				throw ParseException("Eof", "css rule value but got empty");
			}
			auto value = stream.GetStringView(firstChar, lastChar);
			stream.SkipSpace();
			if (stream.Current() != ';' && stream.Current() != 0)
			{
				throw ParseException(stream.Current(), "css rule value");
			}
			_rules.Set(name, std::string(value));
			stream.SkipSpace();
			if (stream.Current() == ';')
			{
				stream.Next();
			}
		}
	}
} // Elpida
