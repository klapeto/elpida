//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgTransform.hpp"

#include "Elpida/Svg/SvgNumber.hpp"
#include "Elpida/Xml/CharacterStream.hpp"

#include <string>

namespace Elpida
{
	template <std::size_t N>
	static void ParseTransformationValues(CharacterStream& stream, SvgFloat (&values)[N])
	{
		std::size_t i = 0;
		while (!stream.Eof())
		{
			stream.SkipSpace();
			auto c = stream.Current();

			if (c == ')')
			{
				return;
			}

			if (c == '(' || c == ',')
			{
				stream.Next();
			}
			stream.SkipSpace();

			if (i == N) throw ParseException("Transforms must have max 6 comma separated numbers");
			values[i++] = SvgNumber::ParseNumber(stream);
		}
		if (i != N - 1)
		{
			throw ParseException("Transform expected " + std::to_string(N) + " numbers");
		}
	}

	static bool ParseNextNumber(CharacterStream& stream, SvgFloat& value)
	{
		while (!stream.Eof())
		{
			stream.SkipSpace();
			auto c = stream.Current();

			if (c == ')')
			{
				return false;
			}

			if (c == '(' || c == ',')
			{
				stream.Next();
			}
			stream.SkipSpace();

			value = SvgNumber::ParseNumber(stream);
			return true;
		}
		return false;
	}

	SvgTransform::SvgTransform(const std::string_view view)
		: t{1.0, 0.0, 0.0, 1.0, 0.0, 0.0}
	{
		try
		{
			CharacterStream stream(view);

			while (!stream.Eof())
			{
				SvgTransform transform;
				stream.SkipSpace();

				switch (stream.Current())
				{
				case 'r':
					if (!stream.ConsumeNextChars("rotate"))
					{
						throw ParseException("Unexpected character: expected 'matrix'");
					}
					{
						SvgFloat angle;
						if (!ParseNextNumber(stream, angle))
						{
							throw ParseException("Unexpected Eof: Expected rotate(angle)");
						}
						SvgFloat x, y;
						bool translate = ParseNextNumber(stream, x) && ParseNextNumber(stream, y);
						if (translate)
						{
							transform.Translate(-x, -y);
						}
						transform.RotateDegrees(angle);
						if (translate)
						{
							transform.Translate(x, y);
						}

						stream.SkipSpace();
						if (stream.Current() != ')')
						{
							throw ParseException("Unexpected Eof: Expected rotate(...). Expected ')' as final character");
						}
					}
					break;
				case 'm':
					if (!stream.ConsumeNextChars("matrix"))
					{
						throw ParseException("Unexpected character: expected 'matrix'");
					}
					{
						SvgFloat t[6];
						ParseTransformationValues(stream, t);
						transform = SvgTransform(t);
					}
					break;
				case 't':
					if (!stream.ConsumeNextChars("translate"))
					{
						throw ParseException("Unexpected character: expected 'translate'");
					}
					{
						SvgFloat tx, ty;
						if (!ParseNextNumber(stream, tx))
						{
							throw ParseException("Unexpected Eof: Expected translate(sx,sy)");
						}
						if (!ParseNextNumber(stream, ty))
						{
							ty = 0.0;
						}
						transform.SetTranslation(tx, ty);

						stream.SkipSpace();
						if (stream.Current() != ')')
						{
							throw ParseException("Unexpected Eof: Expected translate(sx,sy). Expected ')' as final character");
						}
					}
					break;
				case 's':
					if (!stream.Next())
					{
						throw ParseException("Unexpected EOF: expected 'scale' or 'skewX' or 'skewY'");
					}
					if (stream.Current() == 'c')
					{
						if (!stream.ConsumeNextChars("cale"))
						{
							throw ParseException("Unexpected character: expected 'scale'");
						}

						SvgFloat sx, sy;
						if (!ParseNextNumber(stream, sx))
						{
							throw ParseException("Unexpected Eof: Expected scale(sx,sy)");
						}
						if (!ParseNextNumber(stream, sy))
						{
							sy = sx;
						}
						transform.SetScale(sx, sy);
						stream.SkipSpace();
						if (stream.Current() != ')')
						{
							throw ParseException("Unexpected Eof: Expected scale(sx,sy). Expected ')' as final character");
						}
					}
					else if (stream.Current() == 'k')
					{
						if (!stream.ConsumeNextChars("kew"))
						{
							throw ParseException("Unexpected character: expected 'skewX' or 'skewY'");
						}
						switch (stream.Current())
						{
						case 'X':
							{
								stream.Next();
								SvgFloat t[1];
								ParseTransformationValues(stream, t);
								transform.SetSkewX(t[0]);
							}
							break;
						case 'Y':
							{
								stream.Next();
								SvgFloat t[1];
								ParseTransformationValues(stream, t);
								transform.SetSkewY(t[0]);
							}
							break;
						default:
							throw ParseException("Unexpected character: expected 'skewX' or 'skewY'");
						}
					}
					else
					{
						throw ParseException("Unexpected character: expected 'scale' or 'skewX' or 'skewY'");
					}
					break;
				default:
					throw ParseException("Unexpected character: expected 'scale' or 'skewX' or 'skewY' or 'matrix'");
				}
				stream.Next();
				stream.SkipSpace();
				PreMultiply(transform);
			}
		}
		catch (const ParseException&)
		{
			AsIdentity();
		}
	}
} // Elpida
