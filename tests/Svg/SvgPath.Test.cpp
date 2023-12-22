//
// Created by klapeto on 22/12/2023.
//

#include <Elpida/Svg/SvgDefs.hpp>
#include <Elpida/Svg/SvgPath.hpp>
#include <Elpida/Xml/XmlParser.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgPathTests, Parse_Single_Success)
{
	const std::string xml = R"(<path d="M 50.869701,65.948649 Z" />)";
	XmlParser parser;

	const auto element = parser.Parse(xml.c_str(), xml.size());

	SvgDefs defs;
	SvgPath path(element, defs);

	EXPECT_EQ(path.GetCommands().size(), 1);

	auto& command = path.GetCommands().at(0);

	EXPECT_EQ(command.GetType(), SvgPathCommandType::MoveTo);
}