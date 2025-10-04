/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 18/12/2023.
//


#include <Elpida/Xml/ParseException.hpp>
#include <Elpida/Xml/XmlParser.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(XmlParserTests, Parse_SimpleElement_Success)
{
	const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
						<a attr1='Lol' attr:2
			=
			"  ha ha "/>)";
	XmlParser parser;

	const auto element = parser.Parse(xml.c_str(), xml.size());

	EXPECT_EQ(element.GetName(), "a");
	EXPECT_TRUE(element.GetContent().empty());
	EXPECT_TRUE(element.GetChildren().empty());

	auto& attributes = element.GetAttributes();
	EXPECT_EQ(attributes.size(), 2);
	EXPECT_EQ(attributes.at("attr1"), "Lol");
	EXPECT_EQ(attributes.at("attr:2"), "  ha ha ");
}

TEST(XmlParserTests, Parse_SimpleContentElement_Success)
{
	const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
						<a>
							Haha
							LOL
						</a>)";
	XmlParser parser;

	const auto element = parser.Parse(xml.c_str(), xml.size());
	auto& child = element.GetChildren().at(0);
	EXPECT_EQ(child.GetContent(), "Haha LOL");
}

TEST(XmlParserTests, Parse_SimpleCDATAElement_Success)
{
	const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
						<a>
							<![CDATA[	Haha
						LOL		]]>
						</a>)";
	XmlParser parser;

	const auto element = parser.Parse(xml.c_str(), xml.size());
	auto& child = element.GetChildren().at(0);
	EXPECT_EQ(child.GetContent(), "Haha LOL");
}


TEST(XmlParserTests, Parse_ChildrenElement_Success)
{
	const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
						<a attr1='Lol' attr:2="  ha ha ">
							<b id="1">
							</b>
							<c id="2"/>
						</a>)";
	XmlParser parser;

	const auto element = parser.Parse(xml.c_str(), xml.size());

	auto& childA = element.GetChildren().at(0);
	EXPECT_EQ(childA.GetName(), "b");

	auto& attributesA = childA.GetAttributes();
	EXPECT_EQ(attributesA.size(), 1);
	EXPECT_EQ(attributesA.at("id"), "1");

	auto& childB = element.GetChildren().at(1);
	EXPECT_EQ(childB.GetName(), "c");

	auto& attributesB = childB.GetAttributes();
	EXPECT_EQ(attributesB.size(), 1);
	EXPECT_EQ(attributesB.at("id"), "2");
}


TEST(XmlParserTests, Parse_NoElement_Error)
{
	const std::string xml = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)";
	XmlParser parser;

	EXPECT_THROW(parser.Parse(xml.c_str(), xml.size()), ParseException);
}

TEST(XmlParserTests, Parse_InvalidSyntax_Error)
{
#define STRS(x) x, sizeof(x) - 1

	XmlParser parser;

	EXPECT_THROW(parser.Parse(STRS(R"(<)")), ParseException);
	EXPECT_THROW(parser.Parse(STRS(R"(<>)")), ParseException);
	EXPECT_THROW(parser.Parse(STRS(R"(<a><)")), ParseException);
	EXPECT_THROW(parser.Parse(STRS(R"(<a b=/>)")), ParseException);
	EXPECT_THROW(parser.Parse(STRS(R"(<a b="v/>)")), ParseException);
	EXPECT_THROW(parser.Parse(STRS(R"(<a><b)")), ParseException);
	EXPECT_THROW(parser.Parse(STRS(R"(<a><b></a>)")), ParseException);
	EXPECT_THROW(parser.Parse(STRS(R"(<a><b>/a>)")), ParseException);
}