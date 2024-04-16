#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TestShell/TestShell.cpp"

class MockSSD : public ISSD {
public:
	MOCK_METHOD(string, read, (), (override));
	MOCK_METHOD(void, write, (int, string), (override));
};

TEST(TestShellTEST, TESTFullRead) {
	MockSSD mock_ssd;
	TestShell shell{ &mock_ssd };

	EXPECT_CALL(mock_ssd, write(1, "0xAAAAAAAA"))
		.Times(99);

	shell.fullwrite();
}