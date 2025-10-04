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
	const SvgStyle style("test: lol; haha: omg   ; kjsdhk-345g   :  test; abc: rule(-561,  13013.13);urks:1");
	auto& rules = style.GetRules();
	EXPECT_EQ(rules.at("test"), "lol");
	EXPECT_EQ(rules.at("haha"), "omg");
	EXPECT_EQ(rules.at("kjsdhk-345g"), "test");
	EXPECT_EQ(rules.at("abc"), "rule(-561,  13013.13)");
	EXPECT_EQ(rules.at("urks"), "1");
}

TEST(SvgStyleTests, Invalid_ThrowsException)
{
	EXPECT_THROW(SvgStyle("test: lol; haha: ; kjsdhk-345g   :  test;"), ParseException);
	EXPECT_THROW(SvgStyle("test: lol haha:"), ParseException);
	EXPECT_THROW(SvgStyle("test: lol:"), ParseException);
	EXPECT_THROW(SvgStyle("test;"), ParseException);
	EXPECT_THROW(SvgStyle("test"), ParseException);
}
