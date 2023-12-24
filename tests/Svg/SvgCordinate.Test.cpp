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

TEST(SvgCordinateTests, Parse_Invalid_Sucess)
{
	EXPECT_THROW(SvgLength("1jksfdjh"), ParseException);
	EXPECT_THROW(SvgLength("13321el"), ParseException);
	EXPECT_THROW(SvgLength("1eo"), ParseException);
	EXPECT_THROW(SvgLength("1po"), ParseException);
	EXPECT_THROW(SvgLength("1io"), ParseException);
	EXPECT_THROW(SvgLength("1mo"), ParseException);
	EXPECT_THROW(SvgLength("1co"), ParseException);
	EXPECT_THROW(SvgLength("1m"), ParseException);
	EXPECT_THROW(SvgLength("1i"), ParseException);
	EXPECT_THROW(SvgLength("1p"), ParseException);
	EXPECT_THROW(SvgLength("1e"), ParseException);
	EXPECT_THROW(SvgLength("1c"), ParseException);
	EXPECT_THROW(SvgLength("1.0.0.0"), ParseException);
}
