//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgPath.hpp"

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/CharacterStream.hpp>
#include <Elpida/Xml/ParseException.hpp>

namespace Elpida
{
	static bool IsNumberic(const char c)
	{
		return SvgNumber::IsNumber(c) || c == '+' || c == '-';
	}

	static void ParseAllNumbers(CharacterStream& stream, std::vector<double>& currentNumbers)
	{
		currentNumbers.clear();
		stream.Next();
		while (!stream.Eof())
		{
			stream.SkipSpace();
			currentNumbers.push_back(SvgNumber::ParseNumber(stream.GetStringViewWhile([](auto c)
			{
				return IsNumberic(c) || c == '.';
			})));
			stream.SkipSpace();
			if (IsNumberic(stream.Current()))
			{
				continue;
			}
			if (stream.Current() == ',')
			{
				stream.Next();
			}
			else
			{
				break;
			}
		}
	}


	SvgPath::SvgPath(const XmlElement& element, SvgDefs& defs)
		: SvgShape(element, defs)
	{
		auto& d = GetProperties().GetValue("d");

		CharacterStream stream(d);

		std::vector<SvgPathCommand::CommandData> currentCommandData;
		std::vector<double> currentNumbers;
		currentNumbers.reserve(8);

		while (!stream.Eof())
		{
			currentCommandData.clear();
			stream.SkipSpace();
			bool absolute = true;
			switch (auto c = stream.Current())
			{
			case 'm':
				absolute = false;
				[[fallthrough]];
			case 'M':
				{
					ParseAllNumbers(stream, currentNumbers);
					if (currentNumbers.empty() || currentNumbers.size() % 2 != 0)
					{
						throw ParseException("moveTo command requires x,y");
					}

					SvgPathCommand::CommandData data{};
					for (std::size_t i = 0; i < currentNumbers.size(); i += 2)
					{
						data.moveToData.x = currentNumbers[i];
						data.moveToData.y = currentNumbers[i + 1];
						currentCommandData.push_back(data);
					}
					_commands.emplace_back(SvgPathCommandType::MoveTo, std::move(currentCommandData), absolute);
				}

				break;
			case 'l':
				absolute = false;
				[[fallthrough]];
			case 'L':
				{
					ParseAllNumbers(stream, currentNumbers);
					if (currentNumbers.empty() || currentNumbers.size() % 2 != 0)
					{
						throw ParseException("lineTo command requires x,y");
					}

					SvgPathCommand::CommandData data{};
					for (std::size_t i = 0; i < currentNumbers.size(); i += 2)
					{
						data.lineToData.x = currentNumbers[i];
						data.lineToData.y = currentNumbers[i + 1];
						currentCommandData.push_back(data);
					}
					_commands.emplace_back(SvgPathCommandType::LineTo, std::move(currentCommandData), absolute);
				}
				break;
			case 'h':
				absolute = false;
				[[fallthrough]];
			case 'H':
				{
					ParseAllNumbers(stream, currentNumbers);

					if (currentNumbers.empty())
					{
						throw ParseException("horizontalLineTo command requires x");
					}

					SvgPathCommand::CommandData data{};
					for (double currentNumber : currentNumbers)
					{
						data.horizontalLineToData.x = currentNumber;
						currentCommandData.push_back(data);
					}
					_commands.emplace_back(SvgPathCommandType::HorizontalLineTo, std::move(currentCommandData),
					                       absolute);
				}
				break;
			case 'v':
				absolute = false;
				[[fallthrough]];
			case 'V':
				{
					ParseAllNumbers(stream, currentNumbers);

					if (currentNumbers.empty())
					{
						throw ParseException("verticallLineTo command requires x");
					}

					SvgPathCommand::CommandData data{};
					for (double currentNumber : currentNumbers)
					{
						data.verticalLineToData.y = currentNumber;
						currentCommandData.push_back(data);
					}
					_commands.emplace_back(SvgPathCommandType::VerticalLineTo, std::move(currentCommandData), absolute);
				}
				break;
			case 'c':
				absolute = false;
				[[fallthrough]];
			case 'C':
				{
					ParseAllNumbers(stream, currentNumbers);

					if (currentNumbers.empty() || currentNumbers.size() % 6 != 0)
					{
						throw ParseException("curveTo command requires x1 y1 x2 y2 x y");
					}

					SvgPathCommand::CommandData data{};
					for (std::size_t i = 0; i < currentNumbers.size(); i += 6)
					{
						data.curveToData.x1 = currentNumbers[i];
						data.curveToData.y1 = currentNumbers[i + 1];
						data.curveToData.x2 = currentNumbers[i + 2];
						data.curveToData.y2 = currentNumbers[i + 3];
						data.curveToData.x = currentNumbers[i + 4];
						data.curveToData.y = currentNumbers[i + 5];
						currentCommandData.push_back(data);
					}
					_commands.emplace_back(SvgPathCommandType::CurveTo, std::move(currentCommandData), absolute);
				}
				break;
			case 's':
				absolute = false;
				[[fallthrough]];
			case 'S':
				{
					ParseAllNumbers(stream, currentNumbers);

					if (currentNumbers.empty() || currentNumbers.size() % 4 != 0)
					{
						throw ParseException("smoothCurveTo command requires x2 y2 x y");
					}

					SvgPathCommand::CommandData data{};
					for (std::size_t i = 0; i < currentNumbers.size(); i += 4)
					{
						data.smoothCurveToData.x2 = currentNumbers[i];
						data.smoothCurveToData.y2 = currentNumbers[i + 1];
						data.smoothCurveToData.x = currentNumbers[i + 2];
						data.smoothCurveToData.y = currentNumbers[i + 3];
						currentCommandData.push_back(data);
					}
					_commands.emplace_back(SvgPathCommandType::SmoothCurveTo, std::move(currentCommandData), absolute);
				}
				break;
			case 'q':
				absolute = false;
				[[fallthrough]];
			case 'Q':
				{
					ParseAllNumbers(stream, currentNumbers);

					if (currentNumbers.empty() || currentNumbers.size() % 4 != 0)
					{
						throw ParseException("quadricCurveTo command requires x2 y2 x y");
					}

					SvgPathCommand::CommandData data{};
					for (std::size_t i = 0; i < currentNumbers.size(); i += 4)
					{
						data.quadraticCurveToData.x1 = currentNumbers[i];
						data.quadraticCurveToData.y1 = currentNumbers[i + 1];
						data.quadraticCurveToData.x = currentNumbers[i + 2];
						data.quadraticCurveToData.y = currentNumbers[i + 3];
						currentCommandData.push_back(data);
					}
					_commands.emplace_back(SvgPathCommandType::QuadraticCurveTo, std::move(currentCommandData),
					                       absolute);
				}
				break;
			case 't':
				absolute = false;
				[[fallthrough]];
			case 'T':
				{
					ParseAllNumbers(stream, currentNumbers);
					if (currentNumbers.empty() || currentNumbers.size() % 2 != 0)
					{
						throw ParseException("smoothQuadricCurveTo command requires x y");
					}

					SvgPathCommand::CommandData data{};
					for (std::size_t i = 0; i < currentNumbers.size(); i += 2)
					{
						data.smoothQuadraticCurveToData.x = currentNumbers[i];
						data.smoothQuadraticCurveToData.y = currentNumbers[i + 1];
						currentCommandData.push_back(data);
					}
					_commands.emplace_back(SvgPathCommandType::SmoothQuadraticCurveTo, std::move(currentCommandData),
					                       absolute);
				}
				break;
			case 'a':
				absolute = false;
				[[fallthrough]];
			case 'A':
				{
					ParseAllNumbers(stream, currentNumbers);

					if (currentNumbers.empty() || currentNumbers.size() % 7 != 0)
					{
						throw ParseException(
							"arc command requires rx ry x-axis-rotation large-arc-flag sweep-flag x y");
					}

					SvgPathCommand::CommandData data{};
					for (std::size_t i = 0; i < currentNumbers.size(); i += 7)
					{
						data.arcData.rX = currentNumbers[i];
						data.arcData.rY = currentNumbers[i + 1];
						data.arcData.xRotation = currentNumbers[i + 2];
						data.arcData.largeArc = currentNumbers[i + 3] == 1;
						data.arcData.sweep = currentNumbers[i + 4] == 1;
						data.arcData.x = currentNumbers[i + 5];
						data.arcData.y = currentNumbers[i + 6];
						currentCommandData.push_back(data);
					}
					_commands.emplace_back(SvgPathCommandType::Arc, std::move(currentCommandData), absolute);
				}
				break;
			case 'Z':
				[[fallthrough]];
			case 'z':
				_closed = true;
				_commands.emplace_back(SvgPathCommandType::ClosePath, std::vector<SvgPathCommand::CommandData>(),
				                       false);
				stream.Next();
				break;
			default:
				throw ParseException(c, "m,M,");
			}

		}
	}
} // Elpida
