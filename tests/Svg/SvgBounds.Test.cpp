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
// Created by klapeto on 31/3/2024.
//

#include <Elpida/Svg/SvgBounds.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgBoundsTest, Contains_Success)
{
	SvgBounds bounds(5,5,10,10);

	EXPECT_TRUE(bounds.Contains(SvgPoint(7.5,7.5)));
	EXPECT_FALSE(bounds.Contains(SvgPoint(0,0)));
}

TEST(SvgBoundsTest, GetWidth_Success)
{
	SvgBounds bounds(-5,0,10,20);

	EXPECT_EQ(bounds.GetWidth(), 15);
}

TEST(SvgBoundsTest, GetHeight_Success)
{
	SvgBounds bounds(0,-10,10,20);

	EXPECT_EQ(bounds.GetHeight(), 30);
}


TEST(SvgBoundsTest, GetMinX_Success)
{
	SvgBounds bounds(-5,-10,10,20);

	EXPECT_EQ(bounds.GetMinX(), -5);
}


TEST(SvgBoundsTest, GetMinY_Success)
{
	SvgBounds bounds(-5,-10,10,20);

	EXPECT_EQ(bounds.GetMinY(), -10);
}


TEST(SvgBoundsTest, GetMaxX_Success)
{
	SvgBounds bounds(-5,-10,10,20);

	EXPECT_EQ(bounds.GetMaxX(), 10);
}

TEST(SvgBoundsTest, GetMaxY_Success)
{
	SvgBounds bounds(-5,-10,10,20);

	EXPECT_EQ(bounds.GetMaxY(), 20);
}