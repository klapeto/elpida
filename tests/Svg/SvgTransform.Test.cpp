//
// Created by klapeto on 19/12/2023.
//

#include <Elpida/Svg/SvgTransform.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgTransformTests, Scale_Valid_Success)
{
	SvgTransform calculated;
	calculated.SetScale(5.32,-8.94);
	const SvgTransform transform("  scale  ( 5.32, -8.94 )  ");
	EXPECT_EQ(transform[0], calculated[0]);
	EXPECT_EQ(transform[1], calculated[1]);
	EXPECT_EQ(transform[2], calculated[2]);
	EXPECT_EQ(transform[3], calculated[3]);
	EXPECT_EQ(transform[4], calculated[4]);
	EXPECT_EQ(transform[5], calculated[5]);
}


TEST(SvgTransformTests, SkewX_Valid_Success)
{
	SvgTransform calculated;
	calculated.SetSkewX(5.32);
	const SvgTransform transform("  skewX  ( 5.32 )  ");
	EXPECT_EQ(transform[0], calculated[0]);
	EXPECT_EQ(transform[1], calculated[1]);
	EXPECT_EQ(transform[2], calculated[2]);
	EXPECT_EQ(transform[3], calculated[3]);
	EXPECT_EQ(transform[4], calculated[4]);
	EXPECT_EQ(transform[5], calculated[5]);
}

TEST(SvgTransformTests, SkewY_Valid_Success)
{
	SvgTransform calculated;
	calculated.SetSkewY(5.32);
	const SvgTransform transform("  skewY  ( 5.32 )  ");
	EXPECT_EQ(transform[0], calculated[0]);
	EXPECT_EQ(transform[1], calculated[1]);
	EXPECT_EQ(transform[2], calculated[2]);
	EXPECT_EQ(transform[3], calculated[3]);
	EXPECT_EQ(transform[4], calculated[4]);
	EXPECT_EQ(transform[5], calculated[5]);
}

TEST(SvgTransformTests, Translate_Valid_Success)
{
	SvgTransform calculated;
	calculated.SetTranslation(5.32, -9331.45);
	const SvgTransform transform("  translate  ( 5.32 ,  -9331.45  )  ");
	EXPECT_EQ(transform[0], calculated[0]);
	EXPECT_EQ(transform[1], calculated[1]);
	EXPECT_EQ(transform[2], calculated[2]);
	EXPECT_EQ(transform[3], calculated[3]);
	EXPECT_EQ(transform[4], calculated[4]);
	EXPECT_EQ(transform[5], calculated[5]);
}

TEST(SvgTransformTests, Rotate_Valid_Success)
{
	SvgTransform calculated;
	calculated.SetRotation(5.32);
	const SvgTransform transform("  rotate  ( 5.32  )  ");
	EXPECT_EQ(transform[0], calculated[0]);
	EXPECT_EQ(transform[1], calculated[1]);
	EXPECT_EQ(transform[2], calculated[2]);
	EXPECT_EQ(transform[3], calculated[3]);
	EXPECT_EQ(transform[4], calculated[4]);
	EXPECT_EQ(transform[5], calculated[5]);
}


TEST(SvgTransformTests, Matrix_Valid_Success)
{
	double values[] = {5.35,813.4,-91.2,466.1,-13.2, -0.05};
	SvgTransform calculated(values);
	const SvgTransform transform("  matrix  (5.35  , 813.4  ,  -91.2 , 466.1  ,  -13.2,   -0.05  )  ");
	EXPECT_EQ(transform[0], calculated[0]);
	EXPECT_EQ(transform[1], calculated[1]);
	EXPECT_EQ(transform[2], calculated[2]);
	EXPECT_EQ(transform[3], calculated[3]);
	EXPECT_EQ(transform[4], calculated[4]);
	EXPECT_EQ(transform[5], calculated[5]);
}

TEST(SvgTransformTests, Matrix_ValidWithSpaces_Success)
{
	double values[] = {5.35,813.4,-91.2,466.1,-13.2, -0.05};
	SvgTransform calculated(values);
	const SvgTransform transform("  matrix  (5.35   813.4   -91.2 , 466.1   -13.2,   -0.05  )  ");
	EXPECT_EQ(transform[0], calculated[0]);
	EXPECT_EQ(transform[1], calculated[1]);
	EXPECT_EQ(transform[2], calculated[2]);
	EXPECT_EQ(transform[3], calculated[3]);
	EXPECT_EQ(transform[4], calculated[4]);
	EXPECT_EQ(transform[5], calculated[5]);
}

TEST(SvgTransformTests, Multiple_Valid_Success)
{
	SvgTransform calculated;
	calculated.SetScale(5.32,-8.94);
	SvgTransform calculated2;
	calculated2.SetTranslation(-0.32,5.83);
	calculated.PreMultiply(calculated2);
	const SvgTransform transform("scale(5.32,-8.94) translate(-0.32,5.83)");
	EXPECT_EQ(transform[0], calculated[0]);
	EXPECT_EQ(transform[1], calculated[1]);
	EXPECT_EQ(transform[2], calculated[2]);
	EXPECT_EQ(transform[3], calculated[3]);
	EXPECT_EQ(transform[4], calculated[4]);
	EXPECT_EQ(transform[5], calculated[5]);
}