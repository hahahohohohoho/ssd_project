#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../TestShell/TestShell.cpp"
#include "../TestShell/SsdDriver.cpp"
#include "../TestShell/Logger.cpp"

#include <fstream>

using namespace testing;


class MockSSD : public ISSD {
public:
	MOCK_METHOD(string, read, (int), (override));
	MOCK_METHOD(void, write, (int, string), (override));
	MOCK_METHOD(void, erase, (int, int), (override));
};

class TestShellTestFixture : public testing::Test {
public:
	void SetUp() override {
		shell = new TestShell(&mock_ssd);

		cinBackup = std::cin.rdbuf();
		std::cin.rdbuf(cinMock.rdbuf());
	}

	void TearDown() override {
		delete shell;

		std::cin.rdbuf(cinBackup);
	}

	void setMockInput(const std::string& input) {
		cinMock.str(input);
		cinMock.clear();
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
	TestShell* shell;

	std::streambuf* cinBackup;
	std::istringstream cinMock;
};

TEST_F(TestShellTestFixture, TestWrite) {
	setMockInput("write 0 0xDEADCODE");
	
	EXPECT_THROW(shell->processCommand(), InvalidInputException);
}

TEST_F(TestShellTestFixture, TestRead) {
	string testvalue = "0x01234567";
	EXPECT_CALL(mock_ssd, read(0))
		.Times(1)
		.WillOnce(Return(string("0x01234567")));

	testing::internal::CaptureStdout();

	setMockInput("read 0");
	shell->processCommand();

	string output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(getValidCount(output, testvalue), 1);
}

TEST_F(TestShellTestFixture, TestFullRead) {
	string testvalue = "0x01234567";
	EXPECT_CALL(mock_ssd, read(AllOf(Ge(0), Le(99))))
		.Times(100)
		.WillRepeatedly(Return(testvalue));

	testing::internal::CaptureStdout();

	setMockInput("fullread");
	shell->processCommand();
	string output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(getValidCount(output, testvalue), 100);
}

TEST_F(TestShellTestFixture, TestFullWrite) {
	string testvalue = "0x01234567";
	EXPECT_CALL(mock_ssd, write(AllOf(Ge(0), Le(99)), testvalue))
		.Times(100);

	setMockInput("fullwrite 0x01234567");
	shell->processCommand();
}

TEST_F(TestShellTestFixture, TestExit) {
	setMockInput("exit");
	EXPECT_THROW(shell->processCommand(), ExitException);
}

TEST_F(TestShellTestFixture, TestHelp) {

	std::stringstream buffer;
	std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());

	setMockInput("help");
	shell->processCommand();

	std::cout.rdbuf(prevcoutbuf);  // std::cout의 원래 버퍼로 복구

	std::locale::global(std::locale("en_US.UTF-8"));
	std::cout.imbue(std::locale());

	string str = "\nCMD > [ CMD ]\n";
	str.append("- write {no} {data} : {data} was recorded in LBA {no}\n");
	str.append("-- {data} : hexadecimal \n");
	str.append("-- ex. write 3 0xAAAABBBB\n");
	str.append("- read {no} : Read LBA {no} times\n");
	str.append("- fullwrite {value} : 0~99 LBA Write\n");
	str.append("- fullread : 0~99 LBA Read\n");
	str.append("- erase {lba} {size}\n");
	str.append("- erase_range {start lba} {end lba}\n");
	str.append("- exit : shell exits\n");
	str.append("- help : Displays how to use each command\n");
	str.append("\n[ TestCases ]");
	str.append("\n- testapp1 : TestScenario_FullWrite");
	str.append("\n- testapp2 : TestScenario_WriteAging\n");

	ASSERT_EQ(str, buffer.str());  // buffer에 저장된 문자열 검증
}

TEST_F(TestShellTestFixture, TestGetLba) {
	setMockInput("read 123456");

	EXPECT_THROW(shell->processCommand(), InvalidInputException);
}

TEST_F(TestShellTestFixture, TestGetLbaNormal) {
	setMockInput("read 98");

	EXPECT_NO_THROW(shell->processCommand(), 98);
}

TEST_F(TestShellTestFixture, TestGetValue) {
	setMockInput("write 1 zxvasd");

	EXPECT_THROW(shell->processCommand(), InvalidInputException);
}

TEST_F(TestShellTestFixture, TestGetValueNormal) {
	setMockInput("write 1 0xABCDABCD");

	EXPECT_NO_THROW(shell->processCommand(), "0xABCDABCD");
}

class SsdDriverTestFixture : public testing::Test {
public:
	void SetUp() override {
		string dummy_file = "result_dummy.txt";
		createDummy(dummy_file);
		ssdDriver = new SSD_Driver(dummy_file);
		shell = new TestShell(ssdDriver);
		testing::internal::CaptureStdout();

		cinBackup = std::cin.rdbuf();
		std::cin.rdbuf(cinMock.rdbuf());
	}

	void TearDown() override {
		delete ssdDriver;
		delete shell;
		std::cin.rdbuf(cinBackup);
	}

	void setMockInput(const std::string& input) {
		cinMock.str(input);
		cinMock.clear();
	}

	void createDummy(string filename) {
		std::ofstream outfile(filename);
		if (!outfile.is_open()) {
			std::cerr << "[error] file open failed." << std::endl;
			return;
		}
		outfile << "0x12345678" << std::endl;
		outfile.close();
	}

	SSD_Driver* ssdDriver;
	TestShell* shell;
	streambuf* cinBackup;
	istringstream cinMock;
};

TEST_F(SsdDriverTestFixture, DummySsdRead) {
	setMockInput("read 1");
	shell->processCommand();

	string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "\nCMD > 0x12345678\n");
}

TEST_F(SsdDriverTestFixture, DummySsdWrite) {
	setMockInput("write 1 0x87654321");
	shell->processCommand();

	string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "\nCMD > SSD.exe W 1 0x87654321\n");

	// EXPECT_THROW 를 사용하여 exitProgram이 호출될 때 예외가 발생하는지 확인
	//EXPECT_THROW(shell.terminateProcess(), std::runtime_error);
}


