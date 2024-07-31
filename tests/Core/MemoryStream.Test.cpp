//
// Created by klapeto on 31/7/2024.
//

#include <Elpida/Core/ElpidaException.hpp>
#include <Elpida/Core/MemoryStream.hpp>
#include <Elpida/Core/DefaultAllocator.hpp>

#include "gtest/gtest.h"

using namespace Elpida;

TEST(MemoryStreamTests, Write_Read_Success)
{
	MemoryStream stream(std::make_shared<DefaultAllocator>());

	unsigned char data[] = "Test Data";

	stream.Write(data, sizeof(data));

	unsigned char out[sizeof(data)];

	auto read = stream.Read(out, sizeof(out));

	ASSERT_EQ(read, sizeof(out));
	ASSERT_TRUE(memcmp(data, out, sizeof(out)) == 0);
	ASSERT_TRUE(stream.Eof());
}

TEST(MemoryStreamTests, Write_Read_ALotOfData_Success)
{
	MemoryStream stream(std::make_shared<DefaultAllocator>());

	unsigned char data[] = "Test Data lkkjl;afdskj;lfdsajk;lfdsakj;lfdsakj;fsfdfsdgfgdsfsdgdsajk;asfdj;klafsdjk;lasfdkjl;asfdkjlafsdkjlfsadk;ljsafd;kljsfda;jklsfad;kljasf;dkljajksfl;dsfdadsfkljkjfdsakjl;dfaskjldafs;;jkldfsaj;kldfsak;jldfsa;kadsjlf;kljdafs;jkldsfa;lkjsdfa;lajksdf;ldskjfas;fdlkjadsf;akljdsfa;kl";

	stream.Write(data, sizeof(data));

	unsigned char out[sizeof(data)];

	auto read = stream.Read(out, sizeof(out));

	ASSERT_EQ(read, sizeof(out));
	ASSERT_TRUE(memcmp(data, out, sizeof(out)) == 0);
	ASSERT_TRUE(stream.Eof());
}


TEST(MemoryStreamTests, Read_NotEnoughData_Success)
{
	MemoryStream stream(std::make_shared<DefaultAllocator>());

	unsigned char data[] = "Test";

	stream.Write(data, sizeof(data));

	unsigned char out[sizeof(data) + 10];

	auto read = stream.Read(out, sizeof(out));

	ASSERT_EQ(read, sizeof(data));
	ASSERT_TRUE(memcmp(data, out, read) == 0);
}


TEST(MemoryStreamTests, Write_Size_Success)
{
	MemoryStream stream(std::make_shared<DefaultAllocator>());

	unsigned char data[] = "Test Data";

	stream.Write(data, sizeof(data));
	ASSERT_EQ(stream.GetSize(), sizeof(data));
}

TEST(MemoryStreamTests, Existing_Read_Success)
{
	unsigned char data[] = "Test Data";

	Memory memory(std::make_shared<DefaultAllocator>());
	memory.Allocate(sizeof(data));
	std::memcpy(memory.GetData(), data, sizeof(data));

	MemoryStream stream(std::move(memory));

	unsigned char out[sizeof(data)];

	auto read = stream.Read(out, sizeof(out));

	ASSERT_EQ(read, sizeof(out));
	ASSERT_TRUE(memcmp(data, out, sizeof(out)) == 0);
	ASSERT_TRUE(stream.Eof());
}