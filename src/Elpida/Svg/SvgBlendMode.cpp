//
// Created by klapeto on 24/3/2024.
//

#include "Elpida/Svg/SvgBlendMode.hpp"

#include <Elpida/Xml/CharacterStream.hpp>

namespace Elpida
{
	SvgBlendMode SvgBlendModeParser::Parse(std::string_view view)
	{
		if (view.empty()) return SvgBlendMode::Normal;

		CharacterStream stream(view);

		stream.SkipSpace();

		switch (stream.Current())
		{
			case 'n':
				stream.Next();
				if (stream.ConsumeNextChars("ormal"))
				{
					return SvgBlendMode::Normal;
				}
				break;
			case 'm':
				stream.Next();
				if (stream.ConsumeNextChars("ultiply"))
				{
					return SvgBlendMode::Multiply;
				}
				break;
			case 's':
				stream.Next();
				switch (stream.Current())
				{
					case 'c':
						stream.Next();
						if (stream.ConsumeNextChars("reen"))
						{
							return SvgBlendMode::Screen;
						}
						break;
					case 'o':
						stream.Next();
						if (stream.ConsumeNextChars("ft-light"))
						{
							return SvgBlendMode::SoftLight;
						}
						break;
					default: break;
				}

				break;
			case 'o':
				stream.Next();
				if (stream.ConsumeNextChars("verlay"))
				{
					return SvgBlendMode::Overlay;
				}
				break;
			case 'd':
				stream.Next();
				switch (stream.Current())
				{
					case 'a':
						stream.Next();
						if (stream.ConsumeNextChars("rken"))
						{
							return SvgBlendMode::Darken;
						}
						break;
					case 'i':
						stream.Next();
						if (stream.ConsumeNextChars("fference"))
						{
							return SvgBlendMode::Difference;
						}
						break;
					default: break;
				}
				break;
			case 'l':
				stream.Next();
				if (stream.ConsumeNextChars("ighten"))
				{
					return SvgBlendMode::Lighten;
				}
				break;
			case 'c':
				stream.Next();
				if (stream.ConsumeNextChars("olor-"))
				{
					switch (stream.Current())
					{
						case 'd':
							stream.Next();
							if (stream.ConsumeNextChars("odge"))
							{
								return SvgBlendMode::ColorDodge;
							}
							break;
						case 'b':
							stream.Next();
							if (stream.ConsumeNextChars("urn"))
							{
								return SvgBlendMode::ColorBurn;
							}
							break;
						default: break;
					}
				}
				break;
			case 'h':
				stream.Next();
				if (stream.ConsumeNextChars("ard-light"))
				{
					return SvgBlendMode::HardLight;
				}
				break;
			case 'e':
				stream.Next();
				if (stream.ConsumeNextChars("xclusion"))
				{
					return SvgBlendMode::Exclusion;
				}
				break;
			default:
				return SvgBlendMode::Normal;
		}

		return SvgBlendMode::Normal;
	}
} // Elpida
