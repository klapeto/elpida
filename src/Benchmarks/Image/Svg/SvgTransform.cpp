//
// Created by klapeto on 7/12/2023.
//

#include "SvgTransform.hpp"

#include <Benchmarks/Image/Xml/CharacterStream.hpp>
#include <Elpida/Core/ElpidaException.hpp>

#include "SvgNumber.hpp"

namespace Elpida
{
	template <std::size_t N>
	static void ParseTransformationValues(CharacterStream& stream, double (&values)[N])
	{
		std::size_t i = 0;
		while (!stream.Eof())
		{
			stream.SkipSpace();
			auto c = stream.Current();

			if (c ==')')
			{
				return;
			}

			if ( c == '(' || c == ',')
			{
				stream.Next();
			}

			if (i == N) throw ElpidaException("Transforms must have max 6 comma separated numbers");
			values[i++] = SvgNumber::ParseNumber(stream);
		}
		if (i != N - 1)
		{
			throw ElpidaException("Transform expected " + std::to_string(N) + " numbers");
		}
	}

	SvgTransform::SvgTransform(const std::string_view view)
		: t{1.0, 0.0, 0.0, 1.0, 0.0, 0.0}
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
					throw ElpidaException("Unexpected character: expected 'matrix'");
				}
				{
					double t[1];
					ParseTransformationValues(stream, t);
					transform.SetRotation(t[0]);
				}
				break;
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
					switch (stream.Current())
					{
					case 'X':
						{
							stream.Next();
							double t[1];
							ParseTransformationValues(stream, t);
							transform.SetSkewX(t[0]);
						}
						break;
					case 'Y':
						{
							stream.Next();
							double t[1];
							ParseTransformationValues(stream, t);
							transform.SetSkewY(t[0]);
						}
						break;
					default:
						throw ElpidaException("Unexpected character: expected 'skewX' or 'skewY'");
					}
				}
				else
				{
					throw ElpidaException("Unexpected character: expected 'scale' or 'skewX' or 'skewY'");
				}
				break;
			default:
				throw ElpidaException("Unexpected character: expected 'scale' or 'skewX' or 'skewY' or 'matrix'");
			}
			stream.Next();
			stream.SkipSpace();
			PreMultiply(transform);
		}
	}
} // Elpida
