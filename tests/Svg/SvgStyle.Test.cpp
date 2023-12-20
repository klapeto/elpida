//
// Created by klapeto on 20/12/2023.
//

#include <Elpida/Svg/SvgStyle.hpp>
#include <Elpida/Xml/ParseException.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgStyleTests, Empty_Valid_Success)
{
	const SvgStyle style("");
	EXPECT_EQ(style.GetRules().size(), 0);
}

TEST(SvgStyleTests, Single_Valid_Success)
{
	const SvgStyle style("test: lol");
	auto& rules = style.GetRules();
	EXPECT_EQ(rules.at("test"), "lol");
}

TEST(SvgStyleTests, Multiple_Valid_Success)
{
	const SvgStyle style("test: lol; haha: omg   ; kjsdhk-345g   :  test; abc: rule(-561,  13013.13)");
	auto& rules = style.GetRules();
	EXPECT_EQ(rules.at("test"), "lol");
	EXPECT_EQ(rules.at("haha"), "omg");
	EXPECT_EQ(rules.at("kjsdhk-345g"), "test");
	EXPECT_EQ(rules.at("abc"), "rule(-561,  13013.13)");
}

TEST(SvgStyleTests, Invalid_ThrowsException)
{
	EXPECT_THROW(SvgStyle("test: lol; haha: ; kjsdhk-345g   :  test;"), ParseException);
	EXPECT_THROW(SvgStyle("test: lol haha:"), ParseException);
	EXPECT_THROW(SvgStyle("test: lol:"), ParseException);
	EXPECT_THROW(SvgStyle("test;"), ParseException);
	EXPECT_THROW(SvgStyle("test"), ParseException);
}
