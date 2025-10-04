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

#include <Elpida/Svg/SvgLength.hpp>
#include <Elpida/Xml/ParseException.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgCordinateTests, Parse_Valid_Sucess)
{
	{
		SvgLength cordinate("153.5432");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Px);
	}

	{
		SvgLength cordinate("  .5432   ");
		EXPECT_EQ(cordinate.GetValue(), 0.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Px);
	}

	{
		SvgLength cordinate("-153.5432px");
		EXPECT_EQ(cordinate.GetValue(), -153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Px);
	}

	{
		SvgLength cordinate("153.5432pt");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Pt);
	}

	{
		SvgLength cordinate("153.5432pc");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Pc);
	}

	{
		SvgLength cordinate("153.5432mm");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Mm);
	}

	{
		SvgLength cordinate("153.5432cm");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Cm);
	}

	{
		SvgLength cordinate("153.5432in");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::In);
	}

	{
		SvgLength cordinate("153.5432%");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Percent);
	}

	{
		SvgLength cordinate("153.5432em");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Em);
	}

	{
		SvgLength cordinate("153.5432ex");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Ex);
	}
}
