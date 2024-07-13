//
// Created by klapeto on 6/5/2024.
//

#include <Elpida/Svg/SvgPathGenerator.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgPathGeneratorTests, MoveToAbsolute_Success)
{
	SvgPathGenerator generator;
	generator.ParsePathData("M2.35,38.189 4.62-23.13.05.65\n"
							" -6.8e-4,0z");
	auto paths = generator.GetPaths();
	EXPECT_EQ(paths.at(0).GetStartPoint(), SvgPoint(2.35, 38.189));
	EXPECT_EQ(paths.at(0).GetCurves().at(0).GetEnd(), SvgPoint(4.62, -23.13));
	EXPECT_EQ(paths.at(0).GetCurves().at(1).GetEnd(), SvgPoint(0.05, 0.65));
	EXPECT_EQ(paths.at(0).GetCurves().at(2).GetEnd(), SvgPoint(-6.8e-4, 0));
	EXPECT_TRUE(paths.at(0).IsClosed());
}
