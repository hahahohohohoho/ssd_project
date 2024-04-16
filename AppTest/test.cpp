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

	testing::internal::CaptureStdout();
	shell->read(0);
	string output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(output, "0x01234567\n");
}

TEST_F(TestShellTestFixture, TESTFullRead) {
	EXPECT_CALL(mock_ssd, read(AllOf(Ge(0), Le(99))))
		.Times(100)
		.WillRepeatedly(Return("0x01234567"));

	shell->fullread();
}

TEST_F(TestShellTestFixture, TESTFullWrite) {
	EXPECT_CALL(mock_ssd, write(AllOf(Ge(0), Le(99)), "0x01234567"))
		.Times(100);

	shell->fullwrite("0x01234567");
}

TEST(TestShellTEST, TestExit) {

	TestExitStrategy testExit;
	TestShell shell;
	shell.setExitStrategy(&testExit);

	// EXPECT_THROW 를 사용하여 exitProgram이 호출될 때 예외가 발생하는지 확인
	EXPECT_THROW(shell.terminateProcess(), std::runtime_error);
}

TEST(TestShellTEST, TestHelp) {

	TestShell shell;
	std::stringstream buffer;
	std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());

	shell.help();

	std::cout.rdbuf(prevcoutbuf);  // std::cout의 원래 버퍼로 복구
	ASSERT_EQ("asdf", buffer.str());  // buffer에 저장된 문자열 검증
}

