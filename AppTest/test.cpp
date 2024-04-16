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

class TestShellTestFixture : public testing::Test {
public:
	void SetUp() override {
		shell = new TestShell(&mock_ssd);
	}
	void TearDown() override{
		delete shell;
	}

	MockSSD mock_ssd;
	TestShell *shell;
};

TEST_F(TestShellTestFixture, TestWrite) {

	EXPECT_CALL(mock_ssd, write(0, "0xDEADCODE"))
		.Times(1);

	shell->write(0, "0xDEADCODE");
}
TEST_F(TestShellTestFixture, TestRead) {

	EXPECT_CALL(mock_ssd, read(0))
		.Times(1)
		.WillOnce(Return(string("0x01234567")));

	EXPECT_EQ(shell->read(0), "0x01234567");
}

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