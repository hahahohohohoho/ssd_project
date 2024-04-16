#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TestShell/TestShell.cpp"
using namespace testing;

class MockSSD : public ISSD {
public:
	MOCK_METHOD(string, read, (int), (override));
	MOCK_METHOD(void, write, (int, string), (override));
};

TEST(TestShellTEST, TESTFullRead) {
	MockSSD mock_ssd;
	TestShell shell{ &mock_ssd };

	EXPECT_CALL(mock_ssd, read(AllOf(Ge(0), Le(99))))
		.Times(100)
		.WillRepeatedly(Return("0xDEADCODE"));

	shell.fullread();
}

TEST(TestShellTEST, TESTFullWrite) {
	MockSSD mock_ssd;
	TestShell shell{ &mock_ssd };

	EXPECT_CALL(mock_ssd, write(AllOf(Ge(0), Le(99)), "0xDEADCODE"))
		.Times(100);


	shell.fullwrite();
}