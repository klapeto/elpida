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

#include <Elpida/Svg/SvgViewBox.hpp>
#include <Elpida/Xml/ParseException.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgViewBoxTests, Empty_SetsZeros)
{
	const SvgViewBox box("");
	EXPECT_EQ(box.GetMinX(), 0);
	EXPECT_EQ(box.GetMinY(), 0);
	EXPECT_EQ(box.GetWidth(), 0);
	EXPECT_EQ(box.GetHeight(), 0);
}

TEST(SvgViewBoxTests, Value_Valid_Sets)
{
	{
		const SvgViewBox box("1 2 3 4");
		EXPECT_EQ(box.GetMinX(), 1);
		EXPECT_EQ(box.GetMinY(), 2);
		EXPECT_EQ(box.GetWidth(), 3);
		EXPECT_EQ(box.GetHeight(), 4);
	}

	{
		const SvgViewBox box("1% 2 3% 4");
		EXPECT_EQ(box.GetMinX(), 1);
		EXPECT_EQ(box.GetMinY(), 2);
		EXPECT_EQ(box.GetWidth(), 3);
		EXPECT_EQ(box.GetHeight(), 4);
	}
}

static void AssertInvalidViewBox(const SvgViewBox& box)
{
	EXPECT_EQ(box.GetMinX(), 0);
	EXPECT_EQ(box.GetMinY(), 0);
	EXPECT_EQ(box.GetWidth(), 0);
	EXPECT_EQ(box.GetHeight(), 0);
}

TEST(SvgViewBoxTests, Value_Invalid_Resets)
{
	AssertInvalidViewBox(SvgViewBox("1 "));
	AssertInvalidViewBox(SvgViewBox("1 2"));
	AssertInvalidViewBox(SvgViewBox("1 2 3"));
	AssertInvalidViewBox(SvgViewBox("1 a 3"));
	AssertInvalidViewBox(SvgViewBox("1akgjajd"));
	AssertInvalidViewBox(SvgViewBox("    "));
}