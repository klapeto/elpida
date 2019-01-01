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

#ifndef TESTS_FILESYSTEM_TEST_CPP_
#define TESTS_FILESYSTEM_TEST_CPP_

#include <gtest/gtest.h>
#include <Elpida/Utilities/FileSystem.hpp>

using namespace Elpida;

TEST(FileSystemTests, ConcatPathsTest)
{
#if _elpida_linux
	ASSERT_STREQ(FileSystem::concatPaths("usr", "share").c_str(), "usr/share");
	ASSERT_STREQ(FileSystem::concatPaths("/usr", "share").c_str(), "/usr/share");
	ASSERT_STREQ(FileSystem::concatPaths("./Elpida", "share").c_str(), "./Elpida/share");
#else
	ASSERT_STREQ(FileSystem::concatPaths("C:", "Users").c_str(), "C:\\Users");
	ASSERT_STREQ(FileSystem::concatPaths("C:", "Users","klapeto").c_str(), "C:\\Users\\klapeto");
	ASSERT_STREQ(FileSystem::concatPaths("klapeto", "Documents").c_str(), "klapeto\\Documents");
#endif
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif /* TESTS_FILESYSTEM_TEST_CPP_ */
