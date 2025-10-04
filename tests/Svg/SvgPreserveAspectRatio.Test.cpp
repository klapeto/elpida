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


#include <Elpida/Svg/SvgPreserveAspectRatio.hpp>
#include <Elpida/Xml/ParseException.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgPreserveAspectRatioTests, Valid_Success)
{
	{
		const SvgPreserveAspectRatio value("none");
		EXPECT_EQ(value.GetType(), SvgAlignType::Meet);
		EXPECT_EQ(value.GetAlignX(), SvgAxisAlignType::None);
		EXPECT_EQ(value.GetAlignY(), SvgAxisAlignType::None);
	}

	{
		const SvgPreserveAspectRatio value("xMinYMin");
		EXPECT_EQ(value.GetType(), SvgAlignType::Meet);
		EXPECT_EQ(value.GetAlignX(), SvgAxisAlignType::Min);
		EXPECT_EQ(value.GetAlignY(), SvgAxisAlignType::Min);
	}

	{
		const SvgPreserveAspectRatio value("xMidYMin");
		EXPECT_EQ(value.GetType(), SvgAlignType::Meet);
		EXPECT_EQ(value.GetAlignX(), SvgAxisAlignType::Mid);
		EXPECT_EQ(value.GetAlignY(), SvgAxisAlignType::Min);
	}

	{
		const SvgPreserveAspectRatio value("xMidYMid");
		EXPECT_EQ(value.GetType(), SvgAlignType::Meet);
		EXPECT_EQ(value.GetAlignX(), SvgAxisAlignType::Mid);
		EXPECT_EQ(value.GetAlignY(), SvgAxisAlignType::Mid);
	}

	{
		const SvgPreserveAspectRatio value("xMaxYMax");
		EXPECT_EQ(value.GetType(), SvgAlignType::Meet);
		EXPECT_EQ(value.GetAlignX(), SvgAxisAlignType::Max);
		EXPECT_EQ(value.GetAlignY(), SvgAxisAlignType::Max);
	}

	{
		const SvgPreserveAspectRatio value("xMidYMin slice");
		EXPECT_EQ(value.GetType(), SvgAlignType::Slice);
		EXPECT_EQ(value.GetAlignX(), SvgAxisAlignType::Mid);
		EXPECT_EQ(value.GetAlignY(), SvgAxisAlignType::Min);
	}

	{
		const SvgPreserveAspectRatio value("  xMidYMin   meet");
		EXPECT_EQ(value.GetType(), SvgAlignType::Meet);
		EXPECT_EQ(value.GetAlignX(), SvgAxisAlignType::Mid);
		EXPECT_EQ(value.GetAlignY(), SvgAxisAlignType::Min);
	}
}

TEST(SvgPreserveAspectRatioTests, Invalid_Resets)
{
	EXPECT_EQ(SvgPreserveAspectRatio("afadsf").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("afadsf").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("afadsf").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYMin LOL").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYMin LOL").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYMin LOL").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("LOLOL LOL").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("LOLOL LOL").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("LOLOL LOL").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xLl").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("xLl").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xLl").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYLOL").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYLOL").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYLOL").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYMid moot").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYMid moot").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYMid moot").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYMid sco").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYMid sco").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMidYMid sco").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMixYMid").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("xMixYMid").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMixYMid").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMadYMid").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("xMadYMid").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMadYMid").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMi").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("xMi").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMi").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMa").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("xMa").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xMa").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xM").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("xM").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("xM").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("x").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("x").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("x").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("n").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("n").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("n").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("na").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("na").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("na").GetAlignY(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("").GetType(), SvgAlignType::Meet);
	EXPECT_EQ(SvgPreserveAspectRatio("").GetAlignX(), SvgAxisAlignType::Mid);
	EXPECT_EQ(SvgPreserveAspectRatio("").GetAlignY(), SvgAxisAlignType::Mid);
}
