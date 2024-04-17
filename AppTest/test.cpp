#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TestShell/TestShell.cpp"
#include "../TestShell/ExitStrategy.cpp"
using namespace testing;

class MockSSD : public ISSD {
public:
	MOCK_METHOD(string, read, (int), (override));
	MOCK_METHOD(void, write, (int, string), (override));
};

class TestShellTestFixture : public testing::Test {
public:
	void SetUp() override {
		shell = new TestShell(&mock_ssd, &testExit);
	}
	void TearDown() override {
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
	TestExitStrategy testExit;
	TestShell* shell;
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

TEST_F(TestShellTestFixture, TestExit) {

	EXPECT_THROW(shell->terminateProcess(), std::runtime_error);
}

TEST_F(TestShellTestFixture, TestHelp) {

	std::stringstream buffer;
	std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());

	shell->help();

	std::cout.rdbuf(prevcoutbuf);  // std::cout의 원래 버퍼로 복구

	std::locale::global(std::locale("en_US.UTF-8"));
	std::cout.imbue(std::locale());

	string str = "- write {no} {data} : {data} was recorded in LBA {no}\n";
	str.append("-- {data} : hexadecimal \n");
	str.append("-- ex. write 3 0xAAAABBBB\n");
	str.append("- read {no} : Read LBA {no} times\n");
	str.append("- exit : shell exits\n");
	str.append("- help : Displays how to use each command\n");

	ASSERT_EQ(str, buffer.str());  // buffer에 저장된 문자열 검증
}

class SsdDriverTestFixture : public testing::Test {
public:
	void SetUp() override {
		ssdDriver = new SSD_Driver("result_dummy.txt");
		shell = new TestShell(ssdDriver);
		testing::internal::CaptureStdout();

	}
	void TearDown() override {
		delete ssdDriver;
		delete shell;
	}

	SSD_Driver* ssdDriver;
	TestShell* shell;
};

TEST_F(SsdDriverTestFixture, DummySsdRead) {
	shell->read(0x1);

	string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "0x12345678\n");
}
TEST_F(SsdDriverTestFixture, DummySsdWrite) {
	shell->write(0x1, "0x87654321");

	string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "ssd.exe write 1 0x87654321\n");

	// EXPECT_THROW 를 사용하여 exitProgram이 호출될 때 예외가 발생하는지 확인
	//EXPECT_THROW(shell.terminateProcess(), std::runtime_error);
}


