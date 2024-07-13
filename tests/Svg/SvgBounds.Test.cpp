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