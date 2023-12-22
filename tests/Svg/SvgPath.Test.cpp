//
// Created by klapeto on 22/12/2023.
//

#include <Elpida/Svg/SvgDefs.hpp>
#include <Elpida/Svg/SvgPath.hpp>
#include <Elpida/Xml/XmlParser.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgPathTests, Parse_Success)
{
	const std::string xml = R"(<path d="M100,133.15" />)";
	XmlParser parser;

	const auto element = parser.Parse(xml.c_str(), xml.size());

	SvgDefs defs;
	SvgPath path(element, defs);
}