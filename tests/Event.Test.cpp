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
 * Event.Test.cpp
 *
 *  Created on: 26 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TESTS_EVENT_TEST_CPP_
#define TESTS_EVENT_TEST_CPP_

#include "Elpida/Event.hpp"

#include <gtest/gtest.h>
#include <string>

#include "Elpida/EventSubscription.hpp"

using namespace Elpida;

TEST(EventTests, SimpleEventInt)
{
	Event<int> event;
	int val = 0;
	int raisedValue = 0;
	event.subscribe([&val, &raisedValue](int x)
	{
		ASSERT_EQ(x, raisedValue);
		val = 5;
	});
	event.raise(raisedValue);
	ASSERT_EQ(val, 5);
}

TEST(EventTests, SimpleEventString)
{
	Event<const std::string&> event;
	int val = 0;
	std::string raisedValue = "Hello";
	event.subscribe([&val, &raisedValue](const std::string& x)
	{
		ASSERT_EQ(x, raisedValue);
		val = 5;
	});
	event.raise(raisedValue);
	ASSERT_EQ(val, 5);
}

TEST(EventTests, Unsubscribe)
{
	Event<int> event;
	int val = 0;
	int raisedValue = 0;
	auto& subscription = event.subscribe([&val, &raisedValue](int x)
	{
		ASSERT_EQ(x, raisedValue);
		val = x;
	});
	event.raise(raisedValue);
	ASSERT_EQ(val, raisedValue);

	subscription.unsubscribe();
	raisedValue = -6;
	auto previousVal = val;
	event.raise(raisedValue);
	ASSERT_EQ(val, previousVal);
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif /* TESTS_EVENT_TEST_CPP_ */
