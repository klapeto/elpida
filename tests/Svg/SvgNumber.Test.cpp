//
// Created by klapeto on 18/12/2023.
//

#include <Elpida/Svg/SvgNumber.hpp>
#include <Elpida/Xml/ParseException.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgNumberTests, StrTol_ParseSuccess)
{
    EXPECT_EQ(SvgNumber::StrTol("11516516"), 11516516);
}

TEST(SvgNumberTests, StrTol_ParseFail)
{
    EXPECT_THROW(SvgNumber::StrTol("11516h516"), ParseException);
}

TEST(SvgNumberTests, StrTol16_ParseSuccess)
{
    EXPECT_EQ(SvgNumber::StrTol16("45AbCdEf"), 0x45AbCdEf);
}

TEST(SvgNumberTests, StrTol16_ParseFail)
{
    EXPECT_THROW(SvgNumber::StrTol16("11516h516"), ParseException);
}

TEST(SvgNumberTests, ParseNumber_Success)
{
    EXPECT_EQ(SvgNumber::ParseNumber("5"), 5);
    EXPECT_EQ(SvgNumber::ParseNumber("51653132"), 51653132);
    EXPECT_EQ(SvgNumber::ParseNumber("1.2464"), 1.2464);
    EXPECT_EQ(SvgNumber::ParseNumber("+87634.45713"), 87634.45713);
    EXPECT_EQ(SvgNumber::ParseNumber(".66464"), 0.66464);
    EXPECT_EQ(SvgNumber::ParseNumber("-5.453"), -5.453);
    EXPECT_EQ(SvgNumber::ParseNumber("-6543132"), -6543132);
    EXPECT_EQ(SvgNumber::ParseNumber("-1.35e+2"), -135);
    EXPECT_EQ(SvgNumber::ParseNumber("+1.35e+2"), 135);
    EXPECT_EQ(SvgNumber::ParseNumber("1.35e-2"), 1.35 * 1.0 / 100);
    EXPECT_EQ(SvgNumber::ParseNumber("-1.7e-4"), -0.00017);
}


TEST(SvgNumberTests, ParseNumber_Fail)
{
    EXPECT_THROW(SvgNumber::ParseNumber("-"), ParseException);
    EXPECT_THROW(SvgNumber::ParseNumber("+"), ParseException);
    EXPECT_THROW(SvgNumber::ParseNumber("+1.35e+"), ParseException);
}

TEST(SvgNumberTests, TryParseNumber_Fail_Success)
{
	double v;
	EXPECT_FALSE(SvgNumber::TryParseNumber("-", v));
	EXPECT_FALSE(SvgNumber::TryParseNumber("+", v));
	EXPECT_FALSE(SvgNumber::TryParseNumber("+1.35e+", v));
	EXPECT_FALSE(SvgNumber::TryParseNumber(" ", v));
	EXPECT_FALSE(SvgNumber::TryParseNumber("ABC", v));
}

TEST(SvgNumberTests, TryParseNumber_Success)
{
	double v;
	EXPECT_TRUE(SvgNumber::TryParseNumber("5", v));
	EXPECT_EQ(v, 5);
}