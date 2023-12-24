//
// Created by klapeto on 22/12/2023.
//

#include <Elpida/Svg/SvgDefs.hpp>
#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgPath.hpp>
#include <Elpida/Xml/ParseException.hpp>
#include <Elpida/Xml/XmlParser.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgPathTests, MoveTo_ValidSingle_Success)
{
	const std::string xml = R"(<svg><path d="M 50.869701,65.948649" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::MoveTo);
	EXPECT_EQ(command.GetData().size(), 1);
	EXPECT_EQ(command.GetData().at(0).moveToData.x, 50.869701);
	EXPECT_EQ(command.GetData().at(0).moveToData.y, 65.948649);
	EXPECT_EQ(command.IsAbsolute(), true);
}

TEST(SvgPathTests, MoveTo_ValidMultiple_Success)
{
	const std::string xml = R"(<svg><path d="    m50.869701,65.948649    61.671337   116.93882 Z" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 2);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::MoveTo);
	EXPECT_EQ(command.GetData().size(), 2);
	EXPECT_EQ(command.GetData().at(0).moveToData.x, 50.869701);
	EXPECT_EQ(command.GetData().at(0).moveToData.y, 65.948649);

	EXPECT_EQ(command.GetData().at(1).moveToData.x, 61.671337);
	EXPECT_EQ(command.GetData().at(1).moveToData.y, 116.93882);
	EXPECT_EQ(command.IsAbsolute(), false);
}


TEST(SvgPathTests, MoveTo_ValidMultiple2_Success)
{
	const std::string xml = R"(<svg><path d="m50.869701,65.948649 M61.671337 116.93882 Z" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 3);

	{
		auto& command = path.GetCommands().at(0);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::MoveTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).moveToData.x, 50.869701);
		EXPECT_EQ(command.GetData().at(0).moveToData.y, 65.948649);
		EXPECT_EQ(command.IsAbsolute(), false);
	}

	{
		auto& command = path.GetCommands().at(1);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::MoveTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).moveToData.x, 61.671337);
		EXPECT_EQ(command.GetData().at(0).moveToData.y, 116.93882);
		EXPECT_EQ(command.IsAbsolute(), true);
	}
}

TEST(SvgPathTests, MoveTo_Invalid_Success)
{
	XmlParser parser;

	std::string xml = R"(<svg><path d="M 50.869701" /></svg>)";
	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());
}

TEST(SvgPathTests, LineTo_ValidSingle_Success)
{
	const std::string xml = R"(<svg><path d="L 50.869701,65.948649" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::LineTo);
	EXPECT_EQ(command.GetData().size(), 1);
	EXPECT_EQ(command.GetData().at(0).lineToData.x, 50.869701);
	EXPECT_EQ(command.GetData().at(0).lineToData.y, 65.948649);
	EXPECT_EQ(command.IsAbsolute(), true);
}

TEST(SvgPathTests, LineTo_ValidMultiple_Success)
{
	const std::string xml = R"(<svg><path d="    l50.869701,65.948649    61.671337   116.93882 Z" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 2);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::LineTo);
	EXPECT_EQ(command.GetData().size(), 2);
	EXPECT_EQ(command.GetData().at(0).lineToData.x, 50.869701);
	EXPECT_EQ(command.GetData().at(0).lineToData.y, 65.948649);

	EXPECT_EQ(command.GetData().at(1).lineToData.x, 61.671337);
	EXPECT_EQ(command.GetData().at(1).lineToData.y, 116.93882);
	EXPECT_EQ(command.IsAbsolute(), false);
}

TEST(SvgPathTests, LineTo_ValidMultiple2_Success)
{
	const std::string xml = R"(<svg><path d="l50.869701,65.948649 L61.671337 116.93882 Z" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 3);

	{
		auto& command = path.GetCommands().at(0);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::LineTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).lineToData.x, 50.869701);
		EXPECT_EQ(command.GetData().at(0).lineToData.y, 65.948649);
		EXPECT_EQ(command.IsAbsolute(), false);
	}

	{
		auto& command = path.GetCommands().at(1);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::LineTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).lineToData.x, 61.671337);
		EXPECT_EQ(command.GetData().at(0).lineToData.y, 116.93882);
		EXPECT_EQ(command.IsAbsolute(), true);
	}
}

TEST(SvgPathTests, LineTo_Invalid_Success)
{
	XmlParser parser;

	std::string xml = R"(<svg><path d="L 50.869701" /></svg>)";
	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());
}

TEST(SvgPathTests, HorizontalLineTo_ValidSingle_Success)
{
	const std::string xml = R"(<svg><path d="H 50.869701" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::HorizontalLineTo);
	EXPECT_EQ(command.GetData().size(), 1);
	EXPECT_EQ(command.GetData().at(0).horizontalLineToData.x, 50.869701);
	EXPECT_EQ(command.IsAbsolute(), true);
}


TEST(SvgPathTests, HorizontalLineTo_ValidMultiple_Success)
{
	const std::string xml = R"(<svg><path d="    h50.869701,65.948649 Z" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 2);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::HorizontalLineTo);
	EXPECT_EQ(command.GetData().size(), 2);
	EXPECT_EQ(command.GetData().at(0).horizontalLineToData.x, 50.869701);
	EXPECT_EQ(command.GetData().at(1).horizontalLineToData.x, 65.948649);
	EXPECT_EQ(command.IsAbsolute(), false);
}

TEST(SvgPathTests, HorizontalLineTo_ValidMultiple2_Success)
{
	const std::string xml = R"(<svg><path d="h50.869701 H61.671337 Z" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 3);

	{
		auto& command = path.GetCommands().at(0);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::HorizontalLineTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).horizontalLineToData.x, 50.869701);
		EXPECT_EQ(command.IsAbsolute(), false);
	}

	{
		auto& command = path.GetCommands().at(1);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::HorizontalLineTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).horizontalLineToData.x, 61.671337);
		EXPECT_EQ(command.IsAbsolute(), true);
	}
}

TEST(SvgPathTests, HorizontalLineTo_Invalid_Success)
{
	XmlParser parser;

	std::string xml = R"(<svg><path d="L " /></svg>)";
	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());
}

TEST(SvgPathTests, VerticalLineTo_ValidSingle_Success)
{
	const std::string xml = R"(<svg><path d="V 50.869701" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::VerticalLineTo);
	EXPECT_EQ(command.GetData().size(), 1);
	EXPECT_EQ(command.GetData().at(0).verticalLineToData.y, 50.869701);
	EXPECT_EQ(command.IsAbsolute(), true);
}


TEST(SvgPathTests, VerticalLineTo_ValidMultiple_Success)
{
	const std::string xml = R"(<svg><path d="    v-50.869701,65.948649 Z" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 2);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::VerticalLineTo);
	EXPECT_EQ(command.GetData().size(), 2);
	EXPECT_EQ(command.GetData().at(0).verticalLineToData.y, -50.869701);
	EXPECT_EQ(command.GetData().at(1).verticalLineToData.y, 65.948649);
	EXPECT_EQ(command.IsAbsolute(), false);
}

TEST(SvgPathTests, VerticalLineTo_ValidMultiple2_Success)
{
	const std::string xml = R"(<svg><path d="v50.869701 V61.671337 Z" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 3);

	{
		auto& command = path.GetCommands().at(0);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::VerticalLineTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).verticalLineToData.y, 50.869701);
		EXPECT_EQ(command.IsAbsolute(), false);
	}

	{
		auto& command = path.GetCommands().at(1);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::VerticalLineTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).verticalLineToData.y, 61.671337);
		EXPECT_EQ(command.IsAbsolute(), true);
	}
}

TEST(SvgPathTests, VerticalLineTo_Invalid_Success)
{
	SvgDefs defs;
	XmlParser parser;

	std::string xml = R"(<svg><path d="V " /></svg>)";
	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());
}


TEST(SvgPathTests, CurveTo_ValidSingle_Success)
{
	const std::string xml = R"(<svg><path d="C 50.869701 468.66 97.464 68.612 956.46 13" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::CurveTo);
	EXPECT_EQ(command.GetData().size(), 1);
	EXPECT_EQ(command.GetData().at(0).curveToData.x1, 50.869701);
	EXPECT_EQ(command.GetData().at(0).curveToData.y1, 468.66);
	EXPECT_EQ(command.GetData().at(0).curveToData.x2, 97.464);
	EXPECT_EQ(command.GetData().at(0).curveToData.y2, 68.612);
	EXPECT_EQ(command.GetData().at(0).curveToData.x, 956.46);
	EXPECT_EQ(command.GetData().at(0).curveToData.y, 13);
	EXPECT_EQ(command.IsAbsolute(), true);
}


TEST(SvgPathTests, CurveTo_ValidMultiple_Success)
{
	const std::string xml = R"(<svg><path d="C 50.869701 468.66 97.464 68.612 956.46 13 64.869701 97.66 45.464 21.612 36.46 88" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::CurveTo);
	EXPECT_EQ(command.GetData().size(), 2);
	EXPECT_EQ(command.GetData().at(0).curveToData.x1, 50.869701);
	EXPECT_EQ(command.GetData().at(0).curveToData.y1, 468.66);
	EXPECT_EQ(command.GetData().at(0).curveToData.x2, 97.464);
	EXPECT_EQ(command.GetData().at(0).curveToData.y2, 68.612);
	EXPECT_EQ(command.GetData().at(0).curveToData.x, 956.46);
	EXPECT_EQ(command.GetData().at(0).curveToData.y, 13);
	EXPECT_EQ(command.GetData().at(1).curveToData.x1, 64.869701);
	EXPECT_EQ(command.GetData().at(1).curveToData.y1, 97.66);
	EXPECT_EQ(command.GetData().at(1).curveToData.x2, 45.464);
	EXPECT_EQ(command.GetData().at(1).curveToData.y2, 21.612);
	EXPECT_EQ(command.GetData().at(1).curveToData.x, 36.46);
	EXPECT_EQ(command.GetData().at(1).curveToData.y, 88);
	EXPECT_EQ(command.IsAbsolute(), true);
}

TEST(SvgPathTests, CurveTo_ValidMultiple2_Success)
{
	const std::string xml = R"(<svg><path d="C 50.869701 468.66 97.464 68.612 956.46 13 c64.869701 97.66 45.464 21.612 36.46 88" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 2);

	{
		auto& command = path.GetCommands().at(0);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::CurveTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).curveToData.x1, 50.869701);
		EXPECT_EQ(command.GetData().at(0).curveToData.y1, 468.66);
		EXPECT_EQ(command.GetData().at(0).curveToData.x2, 97.464);
		EXPECT_EQ(command.GetData().at(0).curveToData.y2, 68.612);
		EXPECT_EQ(command.GetData().at(0).curveToData.x, 956.46);
		EXPECT_EQ(command.GetData().at(0).curveToData.y, 13);
		EXPECT_EQ(command.IsAbsolute(), true);
	}

	{
		auto& command = path.GetCommands().at(1);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::CurveTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).curveToData.x1, 64.869701);
		EXPECT_EQ(command.GetData().at(0).curveToData.y1, 97.66);
		EXPECT_EQ(command.GetData().at(0).curveToData.x2, 45.464);
		EXPECT_EQ(command.GetData().at(0).curveToData.y2, 21.612);
		EXPECT_EQ(command.GetData().at(0).curveToData.x, 36.46);
		EXPECT_EQ(command.GetData().at(0).curveToData.y, 88);
		EXPECT_EQ(command.IsAbsolute(), false);
	}
}

TEST(SvgPathTests, CurveTo_Invalid_Success)
{
	SvgDefs defs;
	XmlParser parser;

	std::string xml = R"(<svg><path d="C " /></svg>)";
	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());
}


TEST(SvgPathTests, SmoothCurveTo_ValidSingle_Success)
{
	const std::string xml = R"(<svg><path d="S 50.869701 468.66 97.464 68.612" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::SmoothCurveTo);
	EXPECT_EQ(command.GetData().size(), 1);
	EXPECT_EQ(command.GetData().at(0).smoothCurveToData.x2, 50.869701);
	EXPECT_EQ(command.GetData().at(0).smoothCurveToData.y2, 468.66);
	EXPECT_EQ(command.GetData().at(0).smoothCurveToData.x, 97.464);
	EXPECT_EQ(command.GetData().at(0).smoothCurveToData.y, 68.612);
	EXPECT_EQ(command.IsAbsolute(), true);
}


TEST(SvgPathTests, SmoothCurveTo_ValidMultiple_Success)
{
	const std::string xml = R"(<svg><path d="S 50.869701 468.66 97.464 68.612 64.869701 97.66 45.464 21.612" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::SmoothCurveTo);
	EXPECT_EQ(command.GetData().size(), 2);
	EXPECT_EQ(command.GetData().at(0).smoothCurveToData.x2, 50.869701);
	EXPECT_EQ(command.GetData().at(0).smoothCurveToData.y2, 468.66);
	EXPECT_EQ(command.GetData().at(0).smoothCurveToData.x, 97.464);
	EXPECT_EQ(command.GetData().at(0).smoothCurveToData.y, 68.612);
	EXPECT_EQ(command.GetData().at(1).smoothCurveToData.x2, 64.869701);
	EXPECT_EQ(command.GetData().at(1).smoothCurveToData.y2, 97.66);
	EXPECT_EQ(command.GetData().at(1).smoothCurveToData.x, 45.464);
	EXPECT_EQ(command.GetData().at(1).smoothCurveToData.y, 21.612);
	EXPECT_EQ(command.IsAbsolute(), true);
}

TEST(SvgPathTests, SmoothCurveTo_ValidMultiple2_Success)
{
	const std::string xml = R"(<svg><path d="S 50.869701 468.66 97.464 68.612 s64.869701 97.66 45.464 21.612" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 2);

	{
		auto& command = path.GetCommands().at(0);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::SmoothCurveTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).smoothCurveToData.x2, 50.869701);
		EXPECT_EQ(command.GetData().at(0).smoothCurveToData.y2, 468.66);
		EXPECT_EQ(command.GetData().at(0).smoothCurveToData.x, 97.464);
		EXPECT_EQ(command.GetData().at(0).smoothCurveToData.y, 68.612);
		EXPECT_EQ(command.IsAbsolute(), true);
	}

	{
		auto& command = path.GetCommands().at(1);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::SmoothCurveTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).smoothCurveToData.x2, 64.869701);
		EXPECT_EQ(command.GetData().at(0).smoothCurveToData.y2, 97.66);
		EXPECT_EQ(command.GetData().at(0).smoothCurveToData.x, 45.464);
		EXPECT_EQ(command.GetData().at(0).smoothCurveToData.y, 21.612);
		EXPECT_EQ(command.IsAbsolute(), false);
	}
}

TEST(SvgPathTests, SmoothCurveTo_Invalid_Success)
{
	SvgDefs defs;
	XmlParser parser;

	std::string xml = R"(<svg><path d="s " /></svg>)";
	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());
}

TEST(SvgPathTests, QuadricCurveTo_ValidSingle_Success)
{
	const std::string xml = R"(<svg><path d="Q 50.869701 468.66 97.464 68.612" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::QuadraticCurveTo);
	EXPECT_EQ(command.GetData().size(), 1);
	EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.x1, 50.869701);
	EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.y1, 468.66);
	EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.x, 97.464);
	EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.y, 68.612);
	EXPECT_EQ(command.IsAbsolute(), true);
}


TEST(SvgPathTests, QuadricCurveTo_ValidMultiple_Success)
{
	const std::string xml = R"(<svg><path d="Q 50.869701 468.66 97.464 68.612 64.869701 97.66 45.464 21.612" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::QuadraticCurveTo);
	EXPECT_EQ(command.GetData().size(), 2);
	EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.x1, 50.869701);
	EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.y1, 468.66);
	EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.x, 97.464);
	EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.y, 68.612);
	EXPECT_EQ(command.GetData().at(1).quadraticCurveToData.x1, 64.869701);
	EXPECT_EQ(command.GetData().at(1).quadraticCurveToData.y1, 97.66);
	EXPECT_EQ(command.GetData().at(1).quadraticCurveToData.x, 45.464);
	EXPECT_EQ(command.GetData().at(1).quadraticCurveToData.y, 21.612);
	EXPECT_EQ(command.IsAbsolute(), true);
}

TEST(SvgPathTests, QuadricSmoothCurveTo_ValidMultiple2_Success)
{
	const std::string xml = R"(<svg><path d="Q 50.869701 468.66 97.464 68.612 q64.869701 97.66 45.464 21.612" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 2);

	{
		auto& command = path.GetCommands().at(0);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::QuadraticCurveTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.x1, 50.869701);
		EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.y1, 468.66);
		EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.x, 97.464);
		EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.y, 68.612);
		EXPECT_EQ(command.IsAbsolute(), true);
	}

	{
		auto& command = path.GetCommands().at(1);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::QuadraticCurveTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.x1, 64.869701);
		EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.y1, 97.66);
		EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.x, 45.464);
		EXPECT_EQ(command.GetData().at(0).quadraticCurveToData.y, 21.612);
		EXPECT_EQ(command.IsAbsolute(), false);
	}
}

TEST(SvgPathTests, QuadricCurveTo_Invalid_Success)
{
	SvgDefs defs;
	XmlParser parser;

	std::string xml = R"(<svg><path d="q " /></svg>)";
	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());
}


TEST(SvgPathTests, SmoothQuadricCurveTo_ValidSingle_Success)
{
	const std::string xml = R"(<svg><path d="T 50.869701,65.948649" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::SmoothQuadraticCurveTo);
	EXPECT_EQ(command.GetData().size(), 1);
	EXPECT_EQ(command.GetData().at(0).smoothQuadraticCurveToData.x, 50.869701);
	EXPECT_EQ(command.GetData().at(0).smoothQuadraticCurveToData.y, 65.948649);
	EXPECT_EQ(command.IsAbsolute(), true);
}

TEST(SvgPathTests, SmoothQuadricCurveTo_ValidMultiple_Success)
{
	const std::string xml = R"(<svg><path d="    t50.869701,65.948649    61.671337   116.93882 Z" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 2);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::SmoothQuadraticCurveTo);
	EXPECT_EQ(command.GetData().size(), 2);
	EXPECT_EQ(command.GetData().at(0).smoothQuadraticCurveToData.x, 50.869701);
	EXPECT_EQ(command.GetData().at(0).smoothQuadraticCurveToData.y, 65.948649);

	EXPECT_EQ(command.GetData().at(1).smoothQuadraticCurveToData.x, 61.671337);
	EXPECT_EQ(command.GetData().at(1).smoothQuadraticCurveToData.y, 116.93882);
	EXPECT_EQ(command.IsAbsolute(), false);
}


TEST(SvgPathTests, SmoothQuadricCurveTo_ValidMultiple2_Success)
{
	const std::string xml = R"(<svg><path d="t50.869701,65.948649 T61.671337 116.93882 Z" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 3);

	{
		auto& command = path.GetCommands().at(0);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::SmoothQuadraticCurveTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).smoothQuadraticCurveToData.x, 50.869701);
		EXPECT_EQ(command.GetData().at(0).smoothQuadraticCurveToData.y, 65.948649);
		EXPECT_EQ(command.IsAbsolute(), false);
	}

	{
		auto& command = path.GetCommands().at(1);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::SmoothQuadraticCurveTo);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).smoothQuadraticCurveToData.x, 61.671337);
		EXPECT_EQ(command.GetData().at(0).smoothQuadraticCurveToData.y, 116.93882);
		EXPECT_EQ(command.IsAbsolute(), true);
	}
}

TEST(SvgPathTests, SmoothQuadricTo_Invalid_Success)
{
	SvgDefs defs;
	XmlParser parser;

	std::string xml = R"(<svg><path d="T 50.869701" /></svg>)";
	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());
}

TEST(SvgPathTests, ArcTo_ValidSingle_Success)
{
	const std::string xml = R"(<svg><path d="A 50.869701 468.66 97.464 1 1 13 -563" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::Arc);
	EXPECT_EQ(command.GetData().size(), 1);
	EXPECT_EQ(command.GetData().at(0).arcData.rX, 50.869701);
	EXPECT_EQ(command.GetData().at(0).arcData.rY, 468.66);
	EXPECT_EQ(command.GetData().at(0).arcData.xRotation, 97.464);
	EXPECT_EQ(command.GetData().at(0).arcData.largeArc, true);
	EXPECT_EQ(command.GetData().at(0).arcData.sweep, true);
	EXPECT_EQ(command.GetData().at(0).arcData.x, 13);
	EXPECT_EQ(command.GetData().at(0).arcData.y, -563);
	EXPECT_EQ(command.IsAbsolute(), true);
}


TEST(SvgPathTests, ArcTo_ValidMultiple_Success)
{
	const std::string xml = R"(<svg><path d="A 50.869701 468.66 97.464 1 1 13 -563 653 115 95 0 0 65 -13" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::Arc);
	EXPECT_EQ(command.GetData().size(), 2);
	EXPECT_EQ(command.GetData().at(0).arcData.rX, 50.869701);
	EXPECT_EQ(command.GetData().at(0).arcData.rY, 468.66);
	EXPECT_EQ(command.GetData().at(0).arcData.xRotation, 97.464);
	EXPECT_EQ(command.GetData().at(0).arcData.largeArc, true);
	EXPECT_EQ(command.GetData().at(0).arcData.sweep, true);
	EXPECT_EQ(command.GetData().at(0).arcData.x, 13);
	EXPECT_EQ(command.GetData().at(0).arcData.y, -563);
	EXPECT_EQ(command.GetData().at(1).arcData.rX, 653);
	EXPECT_EQ(command.GetData().at(1).arcData.rY, 115);
	EXPECT_EQ(command.GetData().at(1).arcData.xRotation, 95);
	EXPECT_EQ(command.GetData().at(1).arcData.largeArc, false);
	EXPECT_EQ(command.GetData().at(1).arcData.sweep, false);
	EXPECT_EQ(command.GetData().at(1).arcData.x, 65);
	EXPECT_EQ(command.GetData().at(1).arcData.y, -13);
	EXPECT_EQ(command.IsAbsolute(), true);
}

TEST(SvgPathTests, ArcTo_ValidMultiple2_Success)
{
	const std::string xml = R"(<svg><path d="A 50.869701 468.66 97.464 1 1 13 -563 a653 115 95 0 0 65 -13" /></svg>)";
	XmlParser parser;

	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());

	EXPECT_EQ(path.GetCommands().size(), 2);

	{
		auto& command = path.GetCommands().at(0);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::Arc);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).arcData.rX, 50.869701);
		EXPECT_EQ(command.GetData().at(0).arcData.rY, 468.66);
		EXPECT_EQ(command.GetData().at(0).arcData.xRotation, 97.464);
		EXPECT_EQ(command.GetData().at(0).arcData.largeArc, true);
		EXPECT_EQ(command.GetData().at(0).arcData.sweep, true);
		EXPECT_EQ(command.GetData().at(0).arcData.x, 13);
		EXPECT_EQ(command.GetData().at(0).arcData.y, -563);
		EXPECT_EQ(command.IsAbsolute(), true);
	}

	{
		auto& command = path.GetCommands().at(1);

		EXPECT_EQ(command.GetType(), SvgPathCommandType::Arc);
		EXPECT_EQ(command.GetData().size(), 1);
		EXPECT_EQ(command.GetData().at(0).arcData.rX, 653);
		EXPECT_EQ(command.GetData().at(0).arcData.rY, 115);
		EXPECT_EQ(command.GetData().at(0).arcData.xRotation, 95);
		EXPECT_EQ(command.GetData().at(0).arcData.largeArc, false);
		EXPECT_EQ(command.GetData().at(0).arcData.sweep, false);
		EXPECT_EQ(command.GetData().at(0).arcData.x, 65);
		EXPECT_EQ(command.GetData().at(0).arcData.y, -13);
		EXPECT_EQ(command.IsAbsolute(), false);
	}
}

TEST(SvgPathTests, ArcTo_Invalid_Success)
{
	SvgDefs defs;
	XmlParser parser;

	std::string xml = R"(<svg><path d="A " /></svg>)";
	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());
}

TEST(SvgPathTests, InvalidCommand_Success)
{
	SvgDefs defs;
	XmlParser parser;

	const std::string xml = R"(<svg><path d="F " /></svg>)";
	const auto document = SvgDocument(parser.Parse(xml.c_str(), xml.size()));
	const auto& path = *dynamic_cast<SvgPath*>(document.GetElement().GetChildren().at(0).get());
}