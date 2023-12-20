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
		EXPECT_EQ(value.GetType(), SvgAlignType::None);
		EXPECT_EQ(value.GetAlignX(), SvgAxisAlignType::Mid);
		EXPECT_EQ(value.GetAlignY(), SvgAxisAlignType::Mid);
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

TEST(SvgPreserveAspectRatioTests, Invalid_Throws)
{
	EXPECT_THROW(SvgPreserveAspectRatio("afadsf"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMidYMin LOL"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("LOLOL LOL"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("xLl"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMidYLOL"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMidYMid moot"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMidYMid sco"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMixYMid"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMadYMid"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMi"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMa"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("xM"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("x"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("n"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio("na"), ParseException);
	EXPECT_THROW(SvgPreserveAspectRatio(""), ParseException);
}