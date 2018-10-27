/*
 * Event.Test.cpp
 *
 *  Created on: 26 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TESTS_EVENT_TEST_CPP_
#define TESTS_EVENT_TEST_CPP_

#include "Elpida/Event.hpp"
#include "Elpida/EventSubscription.hpp"
#include "Elpida/Types/String.hpp"
#include <gtest/gtest.h>

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
	Event<const String&> event;
	int val = 0;
	String raisedValue = "Hello";
	event.subscribe([&val, &raisedValue](const String& x)
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
