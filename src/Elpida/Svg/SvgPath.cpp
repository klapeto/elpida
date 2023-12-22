//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgPath.hpp"

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/CharacterStream.hpp>
#include <Elpida/Xml/ParseException.hpp>

namespace Elpida
{
	SvgPath::SvgPath(const XmlElement& element, SvgDefs& defs)
		: SvgShape(element, defs)
	{
		auto& d = GetProperties().GetValue("d");

		CharacterStream stream(d);

		auto numberPredicate = [](auto c)
		{
			return SvgNumber::IsNumber(c) || c == '+' || c == '-' || c == '.';
		};

		std::vector<SvgPathCommand::CommandData> currentCommandData;

		while (!stream.Eof())
		{
			stream.SkipSpace();
			bool absolute = true;
			auto c = stream.Current();
			switch (c)
			{
			case 'm':
				absolute = false;
			case 'M':
				stream.Next();
				do
				{
					SvgPathCommand::CommandData data{};
					data.moveToData.x = SvgNumber(stream.GetStringViewWhile(numberPredicate));
					stream.Skip([](auto c) { return CharacterStream::IsSpace(c) || c == ','; });
					data.moveToData.y = SvgNumber(stream.GetStringViewWhile(numberPredicate));
					c = stream.Current();
				}
				while (CharacterStream::IsSpace(c) || c == ',');
				break;
			default:
				throw ParseException(c, "m,M,");
			}

			stream.Next();
		}
	}
} // Elpida
