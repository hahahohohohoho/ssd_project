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

	int getValidCount(string output, string value) {
		int count = 0;
		size_t pos = output.find(value);
		while (pos != string::npos) {
			count++;
			pos = output.find(value, pos + value.length());
		}
		return count;
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

	testing::internal::CaptureStdout();
	shell->read(0);
	string output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(output, "0x01234567\n");
}

TEST_F(TestShellTestFixture, TESTFullRead) {
	string testvalue = "0x01234567";
	EXPECT_CALL(mock_ssd, read(AllOf(Ge(0), Le(99))))
		.Times(100)
		.WillRepeatedly(Return(testvalue));

	testing::internal::CaptureStdout();
	shell->fullread();
	string output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(getValidCount(output, testvalue), 100);

}

TEST_F(TestShellTestFixture, TESTFullWrite) {
	string testvalue = "0x01234567";
	EXPECT_CALL(mock_ssd, write(AllOf(Ge(0), Le(99)), testvalue))
		.Times(100);

	shell->fullwrite(testvalue);
}