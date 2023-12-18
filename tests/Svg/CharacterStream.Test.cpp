//
// Created by klapeto on 18/12/2023.
//

#include <Elpida/Core/ElpidaException.hpp>
#include <Xml/CharacterStream.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(CharacterStreamTests, Eof_EmptyString_ReturnsTrue)
{
	EXPECT_TRUE(CharacterStream("").Eof());
}

TEST(CharacterStreamTests, Eof_NullString_ReturnsTrue)
{
	EXPECT_TRUE(CharacterStream(nullptr, 0).Eof());
}

TEST(CharacterStreamTests, Eof_NonEmpty_ReturnsFalse)
{
	EXPECT_FALSE(CharacterStream("A").Eof());
}

TEST(CharacterStreamTests, Current_NonEmpty_ReturnsChar)
{
	const CharacterStream stream("ABC");

	EXPECT_EQ(stream.Current(), 'A');
}

TEST(CharacterStreamTests, Current_Empty_ReturnsZero)
{
	const CharacterStream stream("");

	EXPECT_EQ(stream.Current(), 0);
}

TEST(CharacterStreamTests, Current_AfterNext_ReturnsNextChar)
{
	CharacterStream stream("ABC");

	stream.Next();
	EXPECT_EQ(stream.Current(), 'B');
}

TEST(CharacterStreamTests, Current_AfterNext_EofReturnsTrue)
{
	CharacterStream stream("A");

	stream.Next();
	EXPECT_TRUE(stream.Eof());
}

TEST(CharacterStreamTests, Next_NonEof_ReturnsTrue)
{
	CharacterStream stream("ABC");

	EXPECT_TRUE(stream.Next());
}

TEST(CharacterStreamTests, Next_Eof_ReturnsFalse)
{
	CharacterStream stream("");

	EXPECT_FALSE(stream.Next());
}

TEST(CharacterStreamTests, GetStringViewWhile_NonEmptyValidPredicate_ReturnsString)
{
	CharacterStream stream("123456ABC");

	const auto view = stream.GetStringViewWhile([](auto c) { return c >= '0' && c <= '9'; });
	EXPECT_EQ(view, "123456");
}

TEST(CharacterStreamTests, GetStringViewWhile_Current_ReturnsNext)
{
	CharacterStream stream("123456ABC");

	stream.GetStringViewWhile([](auto c) { return c >= '0' && c <= '9'; });

	EXPECT_EQ(stream.Current(), 'A');
}

TEST(CharacterStreamTests, GetStringViewWhile_NonEmptyInValidPredicate_ReturnsString)
{
	CharacterStream stream("123456ABC");

	const auto view = stream.GetStringViewWhile([](auto c) { return false; });
	EXPECT_EQ(view, "");
}

TEST(CharacterStreamTests, GetStringViewWhile_Complete_ReturnsWholeString)
{
	CharacterStream stream("123456ABC");

	const auto view = stream.GetStringViewWhile([](auto c) { return true; });

	EXPECT_EQ(view, "123456ABC");
	EXPECT_TRUE(stream.Eof());
}

TEST(CharacterStreamTests, SkipSpace_SpaceExists_Skips)
{
	CharacterStream stream("		\n	\n123456ABC");

	stream.SkipSpace();
	EXPECT_EQ(stream.Current(), '1');
}

TEST(CharacterStreamTests, SkipSpace_NoSpaceExists_DontSkip)
{
	CharacterStream stream("123456ABC");

	stream.SkipSpace();
	EXPECT_EQ(stream.Current(), '1');
}

TEST(CharacterStreamTests, ConsumeNextChars_ValidChars_ConsumesAndReturnsTrue)
{
	CharacterStream stream("ABC");

	EXPECT_TRUE(stream.ConsumeNextChars("ABC"));
	EXPECT_EQ(stream.Current(), 0);
	EXPECT_TRUE(stream.Eof());
}

TEST(CharacterStreamTests, ConsumeNextChars_InvalidChars_ConsumesAndReturnsFalse)
{
	CharacterStream stream("ABC");

	EXPECT_FALSE(stream.ConsumeNextChars("A1"));
	EXPECT_EQ(stream.Current(), 'B');
}

TEST(CharacterStreamTests, ConsumeNextChars_Empty_ConsumesAndReturnsFalse)
{
	CharacterStream stream("");

	EXPECT_FALSE(stream.ConsumeNextChars("A1"));
}

TEST(CharacterStreamTests, ConsumeNextCharsCond_ValidChars_ConsumesAndReturnsTrue)
{
	CharacterStream stream("ABC");

	EXPECT_TRUE(stream.ConsumeNextCharsCond("ABC"));
	EXPECT_EQ(stream.Current(), 0);
	EXPECT_TRUE(stream.Eof());
}

TEST(CharacterStreamTests, ConsumeNextCharsCond_InvalidChars_DoesNotConsumeAndReturnsFalse)
{
	CharacterStream stream("ABC");

	EXPECT_FALSE(stream.ConsumeNextCharsCond("AB1"));
	EXPECT_EQ(stream.Current(), 'A');
	EXPECT_FALSE(stream.Eof());
}

TEST(CharacterStreamTests, GetStringView_ValidRange_ReturnsString)
{
	CharacterStream stream("ABCDEF");

	const auto view = stream.GetStringView(1 , 3);
	EXPECT_EQ(view, "BC");
}

TEST(CharacterStreamTests, GetStringView_InvalidRange_ReturnsEmptyString)
{
	CharacterStream stream("ABCDEF");

	const auto view = stream.GetStringView(876 , 1234);
	EXPECT_EQ(view, "");
}


TEST(CharacterStreamTests, SkipUntilString_Valid_ReturnsTrue)
{
	CharacterStream stream("ABCDEF");

	EXPECT_TRUE(stream.SkipUntilString("CD"));
	EXPECT_EQ(stream.Current(), 'E');
}

TEST(CharacterStreamTests, SkipUntilString_Invalid_ReturnsTrue)
{
	CharacterStream stream("ABCDEF");

	EXPECT_FALSE(stream.SkipUntilString("123"));
	EXPECT_TRUE(stream.Eof());
}

TEST(CharacterStreamTests, Char_ValidIndex_ReturnsChar)
{
	const CharacterStream stream("ABCDEF");

	EXPECT_EQ(stream.Char(2), 'C');
	EXPECT_EQ(stream.Char(3), 'D');
	EXPECT_EQ(stream.Current(), 'A');
}

TEST(CharacterStreamTests, Char_InvalidIndex_ReturnsChar)
{
	const CharacterStream stream("ABCDEF");

	EXPECT_EQ(stream.Char(5455), 0);
}