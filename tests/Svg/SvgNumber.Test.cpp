//
// Created by klapeto on 18/12/2023.
//

#include <Elpida/Core/ElpidaException.hpp>
#include <Svg/SvgNumber.hpp>

#include "gtest/gtest.h"

TEST(SvgNumberTests, StrTolParseSuccess)
{
    EXPECT_EQ(Elpida::SvgNumber::StrTol("11516516"), 11516516);
}

TEST(SvgNumberTests, StrTolParseFail)
{
    EXPECT_THROW(Elpida::SvgNumber::StrTol("11516h516"), Elpida::ElpidaException);
}

TEST(SvgNumberTests, StrTol16ParseSuccess)
{
    EXPECT_EQ(Elpida::SvgNumber::StrTol16("45AbCdEf"), 0x45AbCdEf);
}

TEST(SvgNumberTests, StrTol16ParseFail)
{
    EXPECT_THROW(Elpida::SvgNumber::StrTol16("11516h516"), Elpida::ElpidaException);
}

TEST(SvgNumberTests, ParseNumberSuccess)
{
    EXPECT_EQ(Elpida::SvgNumber::ParseNumber("5"), 5);
    EXPECT_EQ(Elpida::SvgNumber::ParseNumber("51653132"), 51653132);
    EXPECT_EQ(Elpida::SvgNumber::ParseNumber("1.2464"), 1.2464);
    EXPECT_EQ(Elpida::SvgNumber::ParseNumber("+87634.45713"), 87634.45713);
    EXPECT_EQ(Elpida::SvgNumber::ParseNumber(".66464"), 0.66464);
    EXPECT_EQ(Elpida::SvgNumber::ParseNumber("-5.453"), -5.453);
    EXPECT_EQ(Elpida::SvgNumber::ParseNumber("-6543132"), -6543132);
    EXPECT_EQ(Elpida::SvgNumber::ParseNumber("-1.35e+2"), -135);
    EXPECT_EQ(Elpida::SvgNumber::ParseNumber("+1.35e+2"), 135);
    EXPECT_EQ(Elpida::SvgNumber::ParseNumber("1.35e-2"), 1.35 * 1 / 100);
}


TEST(SvgNumberTests, ParseNumberFail)
{
    EXPECT_THROW(Elpida::SvgNumber::ParseNumber("-"), Elpida::ElpidaException);
    EXPECT_THROW(Elpida::SvgNumber::ParseNumber("+"), Elpida::ElpidaException);
    EXPECT_THROW(Elpida::SvgNumber::ParseNumber("+1.35e+"), Elpida::ElpidaException);
}