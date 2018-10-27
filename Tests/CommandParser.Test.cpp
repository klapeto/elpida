/*
 * CommandParser.Test.hpp
 *
 *  Created on: 26 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TESTS_COMMANDPARSER_TEST_CPP_
#define TESTS_COMMANDPARSER_TEST_CPP_

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


#endif /* TESTS_COMMANDPARSER_TEST_CPP_ */
