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
// Created by klapeto on 19/12/2023.
//

#include <Elpida/Svg/SvgTransform.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

static void AssertTransformsEqual(const SvgTransform& a, const SvgTransform& b){
	EXPECT_EQ(a[0], b[0]);
	EXPECT_EQ(a[1], b[1]);
	EXPECT_EQ(a[2], b[2]);
	EXPECT_EQ(a[3], b[3]);
	EXPECT_EQ(a[4], b[4]);
	EXPECT_EQ(a[5], b[5]);
}

TEST(SvgTransformTests, Scale_Valid_Success)
{
	SvgTransform calculated;
	calculated.SetScale(5.32,-8.94);
	const SvgTransform transform("  scale  ( 5.32, -8.94 )  ");
	AssertTransformsEqual(transform, calculated);
}

TEST(SvgTransformTests, Scale_Single_Success)
{
	SvgTransform calculated;
	calculated.SetScale(5.32,5.32);
	const SvgTransform transform("  scale  ( 5.32 )  ");
	AssertTransformsEqual(transform, calculated);
}

TEST(SvgTransformTests, Scale_Invalid_RevertsToIdentity)
{
	SvgTransform calculated;
	calculated.AsIdentity();
	AssertTransformsEqual(SvgTransform("scale()"), calculated);
	AssertTransformsEqual(SvgTransform("skewY(1,2,3)"), calculated);
}

TEST(SvgTransformTests, SkewX_Valid_Success)
{
	SvgTransform calculated;
	calculated.SetSkewX(5.32);
	const SvgTransform transform("  skewX  ( 5.32 )  ");
	AssertTransformsEqual(transform, calculated);
}

TEST(SvgTransformTests, SkewX_Invalid_RevertsToIdentity)
{
	SvgTransform calculated;
	calculated.AsIdentity();
	const SvgTransform transform("skewX()");
	AssertTransformsEqual(transform, calculated);
}

TEST(SvgTransformTests, SkewY_Valid_Success)
{
	SvgTransform calculated;
	calculated.SetSkewY(5.32);
	const SvgTransform transform("  skewY  ( 5.32 )  ");
	AssertTransformsEqual(transform, calculated);
}

TEST(SvgTransformTests, SkewY_Invalid_RevertsToIdentity)
{
	SvgTransform calculated;
	calculated.AsIdentity();
	AssertTransformsEqual(SvgTransform("skewY()"), calculated);
	AssertTransformsEqual(SvgTransform("skewY(1,2)"), calculated);
	AssertTransformsEqual(SvgTransform("skewY(1,2,3)"), calculated);
}

TEST(SvgTransformTests, Translate_Valid_Success)
{
	SvgTransform calculated;
	calculated.SetTranslation(5.32, -9331.45);
	const SvgTransform transform("  translate  ( 5.32 ,  -9331.45  )  ");
	AssertTransformsEqual(transform, calculated);
}

TEST(SvgTransformTests, Translate_Single_Success)
{
	SvgTransform calculated;
	calculated.SetTranslation(5.32, 0);
	const SvgTransform transform("  translate  ( 5.32  )  ");
	AssertTransformsEqual(transform, calculated);
}

TEST(SvgTransformTests, Translate_Invalid_RevertsToIdentity)
{
	SvgTransform calculated;
	calculated.AsIdentity();
	AssertTransformsEqual(SvgTransform("translate()"), calculated);
	AssertTransformsEqual(SvgTransform("translate(1,2,3)"), calculated);
}

TEST(SvgTransformTests, Rotate_Valid_Success)
{
	SvgTransform calculated;
	calculated.RotateDegrees(5.32);
	const SvgTransform transform("  rotate  ( 5.32  )  ");
	AssertTransformsEqual(transform, calculated);
}

TEST(SvgTransformTests, Rotate_WithPoint_Valid_Success)
{
	SvgTransform calculated;
	calculated.Translate(-5, -6);
	calculated.RotateDegrees(5.32);
	calculated.Translate(5,6);
	const SvgTransform transform("  rotate  ( 5.32, 5, 6  )  ");
	AssertTransformsEqual(transform, calculated);
}


TEST(SvgTransformTests, Rotate_Invalid_RevertsToIdentity)
{
	SvgTransform calculated;
	calculated.AsIdentity();
	AssertTransformsEqual(SvgTransform("rotate()"), calculated);
	AssertTransformsEqual(SvgTransform("rotate(1,2,3,4)"), calculated);
}

TEST(SvgTransformTests, Matrix_Valid_Success)
{
	SvgFloat values[] = {5.35,813.4,-91.2,466.1,-13.2, -0.05};
	SvgTransform calculated(values);
	const SvgTransform transform("  matrix  (5.35  , 813.4  ,  -91.2 , 466.1  ,  -13.2,   -0.05  )  ");
	AssertTransformsEqual(transform, calculated);
}

TEST(SvgTransformTests, Matrix_ValidWithSpaces_Success)
{
	SvgFloat values[] = {5.35,813.4,-91.2,466.1,-13.2, -0.05};
	SvgTransform calculated(values);
	const SvgTransform transform("  matrix  (5.35   813.4   -91.2 , 466.1   -13.2,   -0.05  )  ");
	AssertTransformsEqual(transform, calculated);
}

TEST(SvgTransformTests, Matrix_Invalid_RevertsToIdentity)
{
	SvgTransform calculated;
	calculated.AsIdentity();
	AssertTransformsEqual(SvgTransform("matrix()"), calculated);
	AssertTransformsEqual(SvgTransform("matrix(1,2,3,4,5,6,7)"), calculated);
}

TEST(SvgTransformTests, Multiple_Valid_Success)
{
	SvgTransform calculated;
	calculated.SetScale(5.32,-8.94);
	SvgTransform calculated2;
	calculated2.SetTranslation(-0.32,5.83);
	calculated.PreMultiply(calculated2);
	const SvgTransform transform("scale(5.32,-8.94) translate(-0.32,5.83)");
	AssertTransformsEqual(transform, calculated);
}

TEST(SvgTransformTests, Invalid_RevertsToIdentity)
{
	SvgTransform calculated;
	calculated.AsIdentity();
	AssertTransformsEqual(SvgTransform("scale(5.32,56))"), calculated);
	AssertTransformsEqual(SvgTransform("scadsfle(5.32,fgd)"), calculated);
	AssertTransformsEqual(SvgTransform("scale()"), calculated);
	AssertTransformsEqual(SvgTransform("scale("), calculated);
	AssertTransformsEqual(SvgTransform("scale(5.32,-8.94) translggate(-0.32,5.83)"), calculated);
	AssertTransformsEqual(SvgTransform("scale(5.32,-8.94) translate()"), calculated);
	AssertTransformsEqual(SvgTransform("scale(5.32,-8.94) translate)"), calculated);
	AssertTransformsEqual(SvgTransform("scale(5.32,-8.94) translate("), calculated);
	AssertTransformsEqual(SvgTransform("trac(5.32,54)"), calculated);
	AssertTransformsEqual(SvgTransform("slol(5.32,54)"), calculated);
	AssertTransformsEqual(SvgTransform("sklol(5.32,54)"), calculated);
	AssertTransformsEqual(SvgTransform("skewO(5.32,54)"), calculated);
	AssertTransformsEqual(SvgTransform("malakas(5.32,54)"), calculated);
	AssertTransformsEqual(SvgTransform("rolf(5.32,54)"), calculated);
}