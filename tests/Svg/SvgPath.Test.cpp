//
// Created by klapeto on 22/12/2023.
//

#include <Elpida/Svg/SvgDefs.hpp>
#include <Elpida/Svg/SvgPath.hpp>
#include <Elpida/Xml/ParseException.hpp>
#include <Elpida/Xml/XmlParser.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgPathTests, MoveTo_ValidSingle_Success)
{
	const std::string xml = R"(<path d="M 50.869701,65.948649" />)";
	XmlParser parser;

	const auto element = parser.Parse(xml.c_str(), xml.size());

	SvgDefs defs;
	const SvgPath path(element, defs);

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
	const std::string xml = R"(<path d="    m50.869701,65.948649    61.671337   116.93882 Z" />)";
	XmlParser parser;

	const auto element = parser.Parse(xml.c_str(), xml.size());

	SvgDefs defs;
	const SvgPath path(element, defs);

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
	const std::string xml = R"(<path d="m50.869701,65.948649 M61.671337 116.93882 Z" />)";
	XmlParser parser;

	const auto element = parser.Parse(xml.c_str(), xml.size());

	SvgDefs defs;
	const SvgPath path(element, defs);

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
	SvgDefs defs;
	XmlParser parser;

	std::string xml = R"(<path d="M 50.869701" />)";
	auto element = parser.Parse(xml.c_str(), xml.size());

	EXPECT_THROW(SvgPath(element, defs), ParseException);

	xml = R"(<path d="M 50.869701,54 689" />)";
	element = parser.Parse(xml.c_str(), xml.size());

	EXPECT_THROW(SvgPath(element, defs), ParseException);

	xml = R"(<path d="M " />)";
	element = parser.Parse(xml.c_str(), xml.size());

	EXPECT_THROW(SvgPath(element, defs), ParseException);

	xml = R"(<path d="M" />)";
	element = parser.Parse(xml.c_str(), xml.size());

	EXPECT_THROW(SvgPath(element, defs), ParseException);
}