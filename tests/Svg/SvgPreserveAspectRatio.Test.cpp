//
// Created by klapeto on 18/12/2023.
//

#include <Elpida/Core/ElpidaException.hpp>
#include <Svg/SvgPreserveAspectRatio.hpp>

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
	EXPECT_THROW(SvgPreserveAspectRatio("afadsf"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMidYMin LOL"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("LOLOL LOL"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("xLl"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMidYLOL"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMidYMid moot"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMidYMid sco"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMixYMid"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMadYMid"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMi"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("xMa"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("xM"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("x"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("n"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio("na"), ElpidaException);
	EXPECT_THROW(SvgPreserveAspectRatio(""), ElpidaException);
}