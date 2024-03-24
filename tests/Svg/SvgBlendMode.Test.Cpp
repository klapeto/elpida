//
// Created by klapeto on 24/3/2024.
//

#include <Elpida/Svg/SvgBlendMode.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(SvgBlendModeTests, Parse_Success)
{
	EXPECT_EQ(ParseBlendMode("normal"), SvgBlendMode::Normal);
}


