//
// Created by klapeto on 24/3/2024.
//

#include <Elpida/Svg/SvgBlendMode.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgBlendModeTests, Parse_Success)
{
	EXPECT_EQ(SvgBlendModeParser::Parse("normal"), SvgBlendMode::Normal);
	EXPECT_EQ(SvgBlendModeParser::Parse("multiply"), SvgBlendMode::Multiply);
	EXPECT_EQ(SvgBlendModeParser::Parse("screen"), SvgBlendMode::Screen);
	EXPECT_EQ(SvgBlendModeParser::Parse("overlay"), SvgBlendMode::Overlay);
	EXPECT_EQ(SvgBlendModeParser::Parse("darken"), SvgBlendMode::Darken);
	EXPECT_EQ(SvgBlendModeParser::Parse("lighten"), SvgBlendMode::Lighten);
	EXPECT_EQ(SvgBlendModeParser::Parse("color-dodge"), SvgBlendMode::ColorDodge);
	EXPECT_EQ(SvgBlendModeParser::Parse("color-burn"), SvgBlendMode::ColorBurn);
	EXPECT_EQ(SvgBlendModeParser::Parse("hard-light"), SvgBlendMode::HardLight);
	EXPECT_EQ(SvgBlendModeParser::Parse("soft-light"), SvgBlendMode::SoftLight);
	EXPECT_EQ(SvgBlendModeParser::Parse("difference"), SvgBlendMode::Difference);
	EXPECT_EQ(SvgBlendModeParser::Parse("exclusion"), SvgBlendMode::Exclusion);
}


TEST(SvgBlendModeTests, ParseFailed_ResetsToNormal)
{
	EXPECT_EQ(SvgBlendModeParser::Parse(""), SvgBlendMode::Normal);
	EXPECT_EQ(SvgBlendModeParser::Parse("   "), SvgBlendMode::Normal);
	EXPECT_EQ(SvgBlendModeParser::Parse("   \t "), SvgBlendMode::Normal);
	EXPECT_EQ(SvgBlendModeParser::Parse("   \t \n "), SvgBlendMode::Normal);
	EXPECT_EQ(SvgBlendModeParser::Parse("jjksfdjksfdjsfd"), SvgBlendMode::Normal);
	EXPECT_EQ(SvgBlendModeParser::Parse("  df465465"), SvgBlendMode::Normal);
}


