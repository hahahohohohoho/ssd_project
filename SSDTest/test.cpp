#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <fstream>
#include <string>

#include "../SSD/Application.cpp"

using namespace std;
using namespace testing;

class MockSSD : public ISSD {
public:
	MOCK_METHOD(void, read, (int), (override));
	MOCK_METHOD(void, write, (int, string), (override));
};

class TestSSDFixture : public Test {
private:
	const string TEST_NAND_FILE_PATH = "nand.txt";
	const string TEST_RESULT_FILE_PATH = "result.txt";

	void initNand() {
		ofstream mNandOs(TEST_NAND_FILE_PATH);
		for (int i = 0; i < 100; i++) {
			mNandOs << "0x00000000" << "\n";
		}
		mNandOs.close();
	}
	void initResult() {
		ofstream mResultOs(TEST_RESULT_FILE_PATH);
		mResultOs << "0x00000000";
		mResultOs.close();
	}
public:
	void SetUp() override {
		initNand();
		initResult();
	}

	void TearDown() override {
		initNand();
		initResult();
	}

	void writeNand(int address, string data) {
		string nandData[100];

		ifstream nandIs("nand.txt");
		string line;
		int index = 0;
		while (getline(nandIs, line) && line != "") {
			nandData[index++] = line;
		}
		nandIs.close();

		nandData[address] = data;

		ofstream nandOs(TEST_NAND_FILE_PATH);
		for (int i = 0; i < 100; i++) {
			nandOs << nandData[i] << "\n";
		}
		nandOs.close();
	}

	string readNand(int address) {
		ifstream nandIs(TEST_NAND_FILE_PATH);
		int lineNumber = 0;
		string line;
		while (getline(nandIs, line)) {
			if (lineNumber == address) {
				nandIs.close();
				return line;
			}
			lineNumber++;
		}
		nandIs.close();
		return "";
	}

	string readResult() {
		ifstream resultIs(TEST_RESULT_FILE_PATH);
		string line;
		getline(resultIs, line);
		resultIs.close();
		return line;
	}
};

TEST_F(TestSSDFixture, normal) {
	SSD ssd;

	ssd.write(3, "0x00000001");

	EXPECT_EQ("0x00000001", readNand(3));
}

TEST_F(TestSSDFixture, ReadEmptySpace) {
	SSD ssd;

	ssd.read(0);

	EXPECT_THAT(readResult(), "0x00000000");
}

TEST_F(TestSSDFixture, ReadZeroLBA) {
	SSD ssd;
	writeNand(0, "0x12345678");

	ssd.read(0);

	EXPECT_THAT(readResult(), "0x12345678");
}

TEST_F(TestSSDFixture, ReadOneLBA) {
	SSD ssd;
	writeNand(0, "0x12345678");
	writeNand(1, "0xABCDEFAB");

	ssd.read(1);

	EXPECT_THAT(readResult(), "0xABCDEFAB");
}

TEST(TestSSD, AppInvalidArgument) {
	MockSSD ssd;
	Application app(&ssd);

	EXPECT_CALL(ssd, read(_))
		.Times(0);
	EXPECT_CALL(ssd, write(_,_))
		.Times(0);

	char* cmd[10] = { "SSD.exe", "E", "0","0x00000000" };
	app.run(4, cmd);
}

TEST(TestSSD, AppLessArgument) {
	MockSSD ssd;
	Application app(&ssd);

	EXPECT_CALL(ssd, read(_))
		.Times(0);
	EXPECT_CALL(ssd, write(_, _))
		.Times(0);

	char* cmd[10] = { "SSD.exe", "R" };
	app.run(2, cmd);
}

TEST(TestSSD, AppArgumentPassing) {
	MockSSD ssd;
	Application app(&ssd);

	EXPECT_CALL(ssd, read(0))
		.Times(1);
	EXPECT_CALL(ssd, write(0, "0x00000000"))
		.Times(1);

	char* cmd1[10] = { "SSD.exe", "R", "0","0x00000000" };
	app.run(4, cmd1);
	char* cmd2[10] = { "SSD.exe", "W", "0","0x00000000" };
	app.run(4, cmd2);
}

TEST(TestSSD, FileReadEmpty) {
	std::ofstream outFile("nand.txt");
	outFile << "" << endl;
	outFile.close();

	DataArrayFile file("nand.txt");
	string buf[1];
	file.readFileLines(buf, 1);
	EXPECT_THAT(buf[0], Eq(""));
}

TEST(TestSSD, FileReadTwoLines) {
	std::ofstream outFile("nand.txt");
	outFile << "ABCD" << endl;
	outFile << "EFGH" << endl;
	outFile.close();

	DataArrayFile file("nand.txt");
	string buf[2];
	file.readFileLines(buf, 2);
	EXPECT_THAT(buf[0], Eq("ABCD"));
	EXPECT_THAT(buf[1], Eq("EFGH"));
}
