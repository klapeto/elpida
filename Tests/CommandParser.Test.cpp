/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * CommandParser.Test.cpp
 *
 *  Created on: 29 Ιουλ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/CommandParser.hpp"

#include <gtest/gtest.h>

TEST(CommandParserTest, NoParsingCommand)
{
	Elpida::CommandParser parser;
	EXPECT_EQ(parser.getNamedArguments().size(), 0);
	EXPECT_EQ(parser.getUnamedArguments().size(), 0);
}

TEST(CommandParserTest, EmptyInput)
{
	Elpida::CommandParser parser;
	parser.parseCommand("");
	EXPECT_EQ(parser.getNamedArguments().size(), 0);
	EXPECT_EQ(parser.getUnamedArguments().size(), 0);
}

TEST(CommandParserTest, WhitespaceInput)
{
	Elpida::CommandParser parser;
	parser.parseCommand(" 	");
	EXPECT_EQ(parser.getNamedArguments().size(), 0);
	EXPECT_EQ(parser.getUnamedArguments().size(), 0);
}

TEST(CommandParserTest, SingleCommand)
{
	Elpida::CommandParser parser;
	parser.parseCommand("run");
	EXPECT_EQ(parser.getNamedArguments().size(), 0);
	EXPECT_EQ(parser.getUnamedArguments().size(), 1);
	EXPECT_EQ(parser.getArgument(0), "run");
}

TEST(CommandParserTest, SingleCommandAndUnamedArguments)
{
	Elpida::CommandParser parser;
	parser.parseCommand("run ImageTasks input output");
	EXPECT_EQ(parser.getNamedArguments().size(), 0);
	EXPECT_EQ(parser.getUnamedArguments().size(), 4);
	EXPECT_EQ(parser.getArgument(0), "run");
	EXPECT_EQ(parser.getArgument(1), "ImageTasks");
	EXPECT_EQ(parser.getArgument(2), "input");
	EXPECT_EQ(parser.getArgument(3), "output");
}

TEST(CommandParserTest, SingleCommandAndNamedArguments)
{
	Elpida::CommandParser parser;
	parser.parseCommand("run -arg1=8 -arg2=nope -arg3");
	EXPECT_EQ(parser.getNamedArguments().size(), 3);
	EXPECT_EQ(parser.getUnamedArguments().size(), 1);
	EXPECT_EQ(parser.getArgument(0), "run");
	EXPECT_EQ(parser.getArgument("arg1"), "8");
	EXPECT_EQ(parser.getArgument("arg2"), "nope");
	EXPECT_EQ(parser.getArgument("arg3"), "true");
}

TEST(CommandParserTest, MixedCommand)
{
	Elpida::CommandParser parser;
	parser.parseCommand("  run -arg1=8 	remove  and  leave -arg2=nope -arg3 enough haha-arg4   -arg4=imarton-pia   -arg5==gg=t-th  ");
	EXPECT_EQ(parser.getNamedArguments().size(), 5);
	EXPECT_EQ(parser.getUnamedArguments().size(), 6);
	EXPECT_EQ(parser.getArgument(0), "run");
	EXPECT_EQ(parser.getArgument(1), "remove");
	EXPECT_EQ(parser.getArgument(2), "and");
	EXPECT_EQ(parser.getArgument(3), "leave");
	EXPECT_EQ(parser.getArgument(4), "enough");
	EXPECT_EQ(parser.getArgument(5), "haha-arg4");
	EXPECT_EQ(parser.getArgument("arg1"), "8");
	EXPECT_EQ(parser.getArgument("arg2"), "nope");
	EXPECT_EQ(parser.getArgument("arg3"), "true");
	EXPECT_EQ(parser.getArgument("arg4"), "imarton-pia");
	EXPECT_EQ(parser.getArgument("arg5"), "=gg=t-th");
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
