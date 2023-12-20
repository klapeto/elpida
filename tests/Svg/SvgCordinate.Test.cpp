//
// Created by klapeto on 18/12/2023.
//

#include <Elpida/Svg/SvgCoordinate.hpp>
#include <Elpida/Xml/ParseException.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgCordinateTests, Parse_Valid_Sucess)
{
	{
		SvgCoordinate cordinate("153.5432");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Px);
	}

	{
		SvgCoordinate cordinate("  .5432   ");
		EXPECT_EQ(cordinate.GetValue(), 0.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Px);
	}

	{
		SvgCoordinate cordinate("-153.5432px");
		EXPECT_EQ(cordinate.GetValue(), -153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Px);
	}

	{
		SvgCoordinate cordinate("153.5432pt");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Pt);
	}

	{
		SvgCoordinate cordinate("153.5432pc");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Pc);
	}

	{
		SvgCoordinate cordinate("153.5432mm");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Mm);
	}

	{
		SvgCoordinate cordinate("153.5432cm");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Cm);
	}

	{
		SvgCoordinate cordinate("153.5432in");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::In);
	}

	{
		SvgCoordinate cordinate("153.5432%");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Percent);
	}

	{
		SvgCoordinate cordinate("153.5432em");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Em);
	}

	{
		SvgCoordinate cordinate("153.5432ex");
		EXPECT_EQ(cordinate.GetValue(), 153.5432);
		EXPECT_EQ(cordinate.GetUnits(), SvgUnits::Ex);
	}
}

TEST(SvgCordinateTests, Parse_Invalid_Sucess)
{
	EXPECT_THROW(SvgCoordinate("1jksfdjh"), ParseException);
	EXPECT_THROW(SvgCoordinate("13321el"), ParseException);
	EXPECT_THROW(SvgCoordinate("1eo"), ParseException);
	EXPECT_THROW(SvgCoordinate("1po"), ParseException);
	EXPECT_THROW(SvgCoordinate("1io"), ParseException);
	EXPECT_THROW(SvgCoordinate("1mo"), ParseException);
	EXPECT_THROW(SvgCoordinate("1co"), ParseException);
	EXPECT_THROW(SvgCoordinate("1m"), ParseException);
	EXPECT_THROW(SvgCoordinate("1i"), ParseException);
	EXPECT_THROW(SvgCoordinate("1p"), ParseException);
	EXPECT_THROW(SvgCoordinate("1e"), ParseException);
	EXPECT_THROW(SvgCoordinate("1c"), ParseException);
	EXPECT_THROW(SvgCoordinate("1.0.0.0"), ParseException);
}
