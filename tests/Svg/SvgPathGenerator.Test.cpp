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
